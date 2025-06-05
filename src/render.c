/**
 * @file render.c
 * @brief Core ray tracing renderer with Lambertian shading
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "render.h"
#include <stdio.h>
#include <math.h>
#include <float.h>

#define MAX_OBJECTS 100
#define T_MIN 0.001f
#define T_MAX FLT_MAX

/**
 * @brief Simple scene structure
 */
typedef struct {
    Hittable objects[MAX_OBJECTS];
    int object_count;
    Vec3 light_position;
    Color light_color;
    Color ambient_color;
} Scene;

static Scene g_scene = {0};

void scene_clear(void) {
    g_scene.object_count = 0;
}

void scene_add_object(Hittable object) {
    if (g_scene.object_count < MAX_OBJECTS) {
        g_scene.objects[g_scene.object_count++] = object;
    }
}

void scene_set_light(Vec3 position, Color color) {
    g_scene.light_position = position;
    g_scene.light_color = color;
}

void scene_set_ambient(Color color) {
    g_scene.ambient_color = color;
}

/**
 * @brief Test ray intersection with all objects in scene
 */
static bool scene_hit(const Ray *ray, float t_min, float t_max, HitRecord *hit_rec) {
    HitRecord temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;
    
    for (int i = 0; i < g_scene.object_count; i++) {
        if (hittable_hit(&g_scene.objects[i], ray, t_min, closest_so_far, &temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *hit_rec = temp_rec;
        }
    }
    
    return hit_anything;
}

/**
 * @brief Calculate Lambertian shading
 */
static Color calculate_lighting(const HitRecord *hit_rec, Color material_color) {
    // Vector from hit point to light
    Vec3 light_dir = vec3_normalize(vec3_sub(g_scene.light_position, hit_rec->point));
    
    // Lambertian diffuse shading: max(0, dot(normal, light_dir))
    float diffuse_strength = fmaxf(0.0f, vec3_dot(hit_rec->normal, light_dir));
    
    // Combine ambient and diffuse lighting
    Color ambient = color_multiply(g_scene.ambient_color, material_color);
    Color diffuse = color_scale(color_multiply(g_scene.light_color, material_color), diffuse_strength);
    
    return color_add(ambient, diffuse);
}

/**
 * @brief Trace a single ray and return color
 */
static Color trace_ray(const Ray *ray) {
    HitRecord hit_rec;
    
    if (scene_hit(ray, T_MIN, T_MAX, &hit_rec)) {
        // Get material color from the object
        // Use the material color stored in HitRecord
        Color material_color = hit_rec.material_color;
        
        return calculate_lighting(&hit_rec, material_color);
    } else {
        // Background gradient (sky)
        Vec3 unit_direction = vec3_normalize(ray->direction);
        float t = 0.5f * (unit_direction.y + 1.0f);
        Color white = color_create(1.0f, 1.0f, 1.0f);
        Color blue = color_create(0.5f, 0.7f, 1.0f);
        return color_lerp(white, blue, t);
    }
}

/**
 * @brief Render the scene
 */
void render_scene(const Camera *camera, FILE *output) {
    // Write PPM header
    fprintf(output, "P3\n");
    fprintf(output, "%d %d\n", camera->image_width, camera->image_height);
    fprintf(output, "255\n");
    
    // Render pixels
    for (int j = camera->image_height - 1; j >= 0; j--) {
        fprintf(stderr, "\rScanlines remaining: %d ", j);
        fflush(stderr);
        
        for (int i = 0; i < camera->image_width; i++) {
            float u, v;
            camera_pixel_to_uv(camera, i, j, &u, &v);
            
            Ray ray = camera_get_ray(camera, u, v);
            Color pixel_color = trace_ray(&ray);
            
            // Write color to output
            uint8_t r, g, b;
            color_to_u8(pixel_color, &r, &g, &b);
            fprintf(output, "%d %d %d\n", r, g, b);
        }
    }
    
    fprintf(stderr, "\nDone.\n");
}