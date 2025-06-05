/**
 * @file camera.c
 * @brief Camera implementation for ray generation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "camera.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Camera camera_create_orthographic(Vec3 origin, Vec3 target, Vec3 up,
                                 float viewport_width, float viewport_height,
                                 int image_width, int image_height) {
    Camera camera;
    camera.origin = origin;
    camera.image_width = image_width;
    camera.image_height = image_height;
    
    // Calculate camera coordinate system
    Vec3 w = vec3_normalize(vec3_sub(origin, target)); // camera looks in -w direction
    Vec3 u = vec3_normalize(vec3_cross(up, w));        // right vector
    Vec3 v = vec3_cross(w, u);                         // up vector
    
    // Set up viewport
    camera.horizontal = vec3_scale(u, viewport_width);
    camera.vertical = vec3_scale(v, viewport_height);
    
    // Lower-left corner
    Vec3 half_horizontal = vec3_scale(camera.horizontal, 0.5f);
    Vec3 half_vertical = vec3_scale(camera.vertical, 0.5f);
    camera.lower_left = vec3_sub(vec3_sub(origin, half_horizontal), half_vertical);
    
    return camera;
}

Camera camera_create_perspective(Vec3 origin, Vec3 target, Vec3 up,
                                float fov, float aspect_ratio,
                                int image_width, int image_height) {
    // Convert FOV to radians
    float theta = fov * M_PI / 180.0f;
    float half_height = tanf(theta / 2.0f);
    float half_width = aspect_ratio * half_height;
    
    // Use orthographic for now - perspective will be future enhancement
    return camera_create_orthographic(origin, target, up,
                                     2.0f * half_width, 2.0f * half_height,
                                     image_width, image_height);
}

Ray camera_get_ray(const Camera *camera, float u, float v) {
    Vec3 horizontal_offset = vec3_scale(camera->horizontal, u);
    Vec3 vertical_offset = vec3_scale(camera->vertical, v);
    Vec3 pixel_pos = vec3_add(vec3_add(camera->lower_left, horizontal_offset), vertical_offset);
    Vec3 direction = vec3_sub(pixel_pos, camera->origin);
    
    return ray_create(camera->origin, direction);
}

void camera_pixel_to_uv(const Camera *camera, int pixel_x, int pixel_y, 
                       float *u, float *v) {
    *u = (float)pixel_x / (float)(camera->image_width - 1);
    *v = (float)pixel_y / (float)(camera->image_height - 1);
}

void camera_print(const Camera *camera) {
    printf("Camera {\n");
    printf("  origin: ");
    vec3_print(camera->origin);
    printf("  lower_left: ");
    vec3_print(camera->lower_left);
    printf("  image_size: %dx%d\n", camera->image_width, camera->image_height);
    printf("}\n");
}