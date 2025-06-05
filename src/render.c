/**
 * @file render.c
 * @brief Rendering pipeline implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "render.h"
#include <stdlib.h>
#include <time.h>

// Simple random number generator state
static unsigned int rng_state = 1;

RenderSettings render_settings_default(void) {
    RenderSettings settings;
    settings.samples_per_pixel = 1;  // No anti-aliasing for performance
    settings.max_depth = 10;
    settings.show_progress = true;
    return settings;
}

void set_random_seed(unsigned int seed) {
    rng_state = seed;
}

float random_float(void) {
    // Linear congruential generator
    rng_state = rng_state * 1664525 + 1013904223;
    return (float)(rng_state % 10000) / 10000.0f;
}

Color render_pixel(const Scene *scene, const Camera *camera, 
                  int pixel_x, int pixel_y, const RenderSettings *settings) {
    Color pixel_color = color_black();
    
    for (int s = 0; s < settings->samples_per_pixel; s++) {
        float u, v;
        
        if (settings->samples_per_pixel == 1) {
            // No anti-aliasing - sample center of pixel
            camera_pixel_to_uv(camera, pixel_x, pixel_y, &u, &v);
        } else {
            // Anti-aliasing - add random offset within pixel
            float offset_x = random_float();
            float offset_y = random_float();
            float adj_x = pixel_x + offset_x;
            float adj_y = pixel_y + offset_y;
            
            u = adj_x / (float)(camera->image_width - 1);
            v = adj_y / (float)(camera->image_height - 1);
        }
        
        Ray ray = camera_get_ray(camera, u, v);
        Color sample_color = scene_ray_color(scene, &ray, settings->max_depth);
        pixel_color = color_add(pixel_color, sample_color);
    }
    
    // Average the samples
    if (settings->samples_per_pixel > 1) {
        pixel_color = color_scale(pixel_color, 1.0f / (float)settings->samples_per_pixel);
    }
    
    return pixel_color;
}

bool render_scene_to_file(const Scene *scene, const Camera *camera, 
                         const RenderSettings *settings, FILE *output_file) {
    if (!output_file || !scene || !camera || !settings) {
        return false;
    }
    
    // Write PPM header
    fprintf(output_file, "P3\n");
    fprintf(output_file, "%d %d\n", camera->image_width, camera->image_height);
    fprintf(output_file, "255\n");
    
    // Set random seed for consistent results
    set_random_seed((unsigned int)time(NULL));
    
    // Render pixels
    for (int j = camera->image_height - 1; j >= 0; j--) {
        if (settings->show_progress) {
            fprintf(stderr, "\rScanlines remaining: %d ", j);
            fflush(stderr);
        }
        
        for (int i = 0; i < camera->image_width; i++) {
            Color pixel_color = render_pixel(scene, camera, i, j, settings);
            
            // Gamma correction (simple gamma = 2.0)
            pixel_color = color_gamma_correct(pixel_color, 2.0f);
            
            // Convert to 8-bit and write
            uint8_t r, g, b;
            color_to_u8(pixel_color, &r, &g, &b);
            fprintf(output_file, "%d %d %d\n", r, g, b);
        }
    }
    
    if (settings->show_progress) {
        fprintf(stderr, "\nDone.\n");
    }
    
    return true;
}