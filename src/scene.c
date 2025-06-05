/**
 * @file scene.c
 * @brief Scene management and rendering implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "scene.h"
#include <stdio.h>
#include <math.h>

#define EPSILON 0.001f

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
    
    // Add small ambient light
    Color ambient = color_scale(material_color, 0.1f);
    final_color = color_add(final_color, ambient);
    
    // Add contribution from each light
    for (int i = 0; i < scene->light_count; i++) {
        const PointLight *light = &scene->lights[i];
        
        Vec3 light_dir = vec3_normalize(vec3_sub(light->position, hit_rec->point));
        float dot_product = vec3_dot(hit_rec->normal, light_dir);
        float lambertian = fmaxf(0.0f, dot_product);
        
        Color light_contribution = color_multiply(material_color, light->color);
        light_contribution = color_scale(light_contribution, lambertian * light->intensity);
        
        final_color = color_add(final_color, light_contribution);
    }
    
    return final_color;
}

Color scene_ray_color(const Scene *scene, const Ray *ray, int depth) {
    if (depth <= 0) {
        return color_black();
    }
    
    HitRecord hit_rec;
    if (scene_hit(scene, ray, EPSILON, INFINITY, &hit_rec)) {
        // Get material color - for now, assume sphere data contains color
        Color material_color = color_white(); // default
        
        // Try to get color from the hit object
        // This is a simplified approach - in a real ray tracer you'd have a proper material system
        for (int i = 0; i < scene->object_count; i++) {
            HitRecord test_rec;
            if (hittable_hit(&scene->objects[i], ray, EPSILON, hit_rec.t + EPSILON, &test_rec)) {
                if (fabsf(test_rec.t - hit_rec.t) < EPSILON) {
                    // This is likely the same object - try to get its color
                    // For spheres, the data pointer contains the sphere structure
                    const void *data = scene->objects[i].data;
                    // Assume it's a sphere for now - in real code you'd have a material system
                    material_color = color_create(0.7f, 0.3f, 0.3f); // default red
                    break;
                }
            }
        }
        
        return scene_shade_lambertian(scene, &hit_rec, material_color);
    }
    
    return scene->background_color;
}

void render_scene(Camera *camera, Scene *scene, FILE *output) {
    // Write PPM header
    fprintf(output, "P3\n");
    fprintf(output, "%d %d\n", camera->image_width, camera->image_height);
    fprintf(output, "255\n");
    
    // Render each pixel
    for (int j = camera->image_height - 1; j >= 0; j--) {
        fprintf(stderr, "\rScanlines remaining: %d ", j);
        fflush(stderr);
        
        for (int i = 0; i < camera->image_width; i++) {
            float u, v;
            camera_pixel_to_uv(camera, i, j, &u, &v);
            
            Ray ray = camera_get_ray(camera, u, v);
            Color pixel_color = scene_ray_color(scene, &ray, 10);
            
            // Convert to PPM format
            uint8_t r, g, b;
            color_to_u8(pixel_color, &r, &g, &b);
            fprintf(output, "%d %d %d\n", r, g, b);
        }
    }
    
    fprintf(stderr, "\nDone.\n");
}

void scene_print(const Scene *scene) {
    printf("Scene {\n");
    printf("  objects: %d\n", scene->object_count);
    printf("  lights: %d\n", scene->light_count);
    printf("  background: ");
    color_print(scene->background_color);
    printf("}\n");
}