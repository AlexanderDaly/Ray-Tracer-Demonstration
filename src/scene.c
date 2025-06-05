/**
 * @file scene.c
 * @brief Scene management implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "scene.h"
#include <stdio.h>
#include <float.h>

Scene scene_create(Color background_color) {
    Scene scene;
    scene.object_count = 0;
    scene.light_count = 0;
    scene.background_color = background_color;
    return scene;
}

bool scene_add_object(Scene *scene, Hittable object) {
    if (scene->object_count >= MAX_OBJECTS) {
        return false;
    }
    scene->objects[scene->object_count] = object;
    scene->object_count++;
    return true;
}

bool scene_add_light(Scene *scene, PointLight light) {
    if (scene->light_count >= 8) {
        return false;
    }
    scene->lights[scene->light_count] = light;
    scene->light_count++;
    return true;
}

bool scene_hit(const Scene *scene, const Ray *ray, float t_min, float t_max, HitRecord *hit_rec) {
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;
    
    for (int i = 0; i < scene->object_count; i++) {
        if (hittable_hit(&scene->objects[i], ray, t_min, closest_so_far, &temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *hit_rec = temp_rec;
        }
    }
    
    return hit_anything;
}

Color scene_shade_lambertian(const Scene *scene, const HitRecord *hit_rec, Color material_color) {
    Color final_color = color_black();
    
    // Ambient lighting (small constant)
    Color ambient = color_scale(material_color, 0.1f);
    final_color = color_add(final_color, ambient);
    
    // Calculate contribution from each light
    for (int i = 0; i < scene->light_count; i++) {
        const PointLight *light = &scene->lights[i];
        
        Vec3 light_dir = vec3_normalize(vec3_sub(light->position, hit_rec->point));
        
        // Check if light is on the same side as the surface normal
        float dot_product = vec3_dot(hit_rec->normal, light_dir);
        if (dot_product > 0.0f) {
            // Simple shadow testing - cast ray to light
            Vec3 shadow_origin = vec3_add(hit_rec->point, vec3_scale(hit_rec->normal, 1e-4f));
            Ray shadow_ray = ray_create(shadow_origin, light_dir);
            
            float light_distance = vec3_length(vec3_sub(light->position, hit_rec->point));
            HitRecord shadow_hit;
            bool in_shadow = scene_hit(scene, &shadow_ray, 1e-4f, light_distance - 1e-4f, &shadow_hit);
            
            if (!in_shadow) {
                // Lambertian diffuse shading: I = I_light * material * dot(N, L)
                Color light_contribution = color_multiply(material_color, light->color);
                light_contribution = color_scale(light_contribution, dot_product * light->intensity);
                
                // Distance attenuation
                float attenuation = 1.0f / (1.0f + 0.1f * light_distance + 0.01f * light_distance * light_distance);
                light_contribution = color_scale(light_contribution, attenuation);
                
                final_color = color_add(final_color, light_contribution);
            }
        }
    }
    
    return color_clamp(final_color);
}

Color scene_ray_color(const Scene *scene, const Ray *ray, int depth) {
    // Prevent infinite recursion
    if (depth <= 0) {
        return color_black();
    }
    
    HitRecord hit_rec;
    if (scene_hit(scene, ray, 0.001f, FLT_MAX, &hit_rec)) {
        // For now, we need to get the material color from the object
        // This is a simplified approach - in a full renderer, materials would be separate
        Color material_color = color_create(0.7f, 0.3f, 0.3f); // Default reddish color
        
        // Try to get color from sphere or plane if possible
        // This is a hack - in a real system, materials would be handled properly
        for (int i = 0; i < scene->object_count; i++) {
            HitRecord temp_hit;
            if (hittable_hit(&scene->objects[i], ray, 0.001f, hit_rec.t + 0.001f, &temp_hit)) {
                if (fabsf(temp_hit.t - hit_rec.t) < 0.001f) {
                    // This is likely the same hit - try to extract color
                    // This is very hacky but works for our demo
                    if (scene->objects[i].data) {
                        // Assume first member is always center/point, and third is color
                        Color *color_ptr = (Color*)((char*)scene->objects[i].data + sizeof(Vec3) + sizeof(float));
                        material_color = *color_ptr;
                    }
                    break;
                }
            }
        }
        
        return scene_shade_lambertian(scene, &hit_rec, material_color);
    }
    
    // No hit - return background color
    return scene->background_color;
}

void scene_print(const Scene *scene) {
    printf("Scene {\n");
    printf("  objects: %d\n", scene->object_count);
    printf("  lights: %d\n", scene->light_count);
    printf("  background: ");
    color_print(scene->background_color);
    printf("}\n");
}