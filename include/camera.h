/**
 * @file camera.h
 * @brief Camera for ray generation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"

/**
 * @brief Camera structure for ray generation
 */
typedef struct {
    Vec3 origin;       ///< Camera position
    Vec3 lower_left;   ///< Lower-left corner of the viewport
    Vec3 horizontal;   ///< Horizontal vector across viewport
    Vec3 vertical;     ///< Vertical vector across viewport
    int image_width;   ///< Image width in pixels
    int image_height;  ///< Image height in pixels
} Camera;

/**
 * @brief Create an orthographic camera
 * @param origin Camera position
 * @param target Point camera is looking at
 * @param up Up vector (typically (0, 1, 0))
 * @param viewport_width Width of the viewport in world units
 * @param viewport_height Height of the viewport in world units
 * @param image_width Image width in pixels
 * @param image_height Image height in pixels
 * @return Configured camera
 */
Camera camera_create_orthographic(Vec3 origin, Vec3 target, Vec3 up,
                                 float viewport_width, float viewport_height,
                                 int image_width, int image_height);

/**
 * @brief Create a perspective camera (for future use)
 * @param origin Camera position
 * @param target Point camera is looking at
 * @param up Up vector
 * @param fov Field of view in degrees
 * @param aspect_ratio Width/height ratio
 * @param image_width Image width in pixels
 * @param image_height Image height in pixels
 * @return Configured camera
 */
Camera camera_create_perspective(Vec3 origin, Vec3 target, Vec3 up,
                                float fov, float aspect_ratio,
                                int image_width, int image_height);

/**
 * @brief Generate ray for given pixel coordinates
 * @param camera The camera
 * @param u Horizontal coordinate [0.0, 1.0]
 * @param v Vertical coordinate [0.0, 1.0]
 * @return Ray from camera through pixel
 */
Ray camera_get_ray(const Camera *camera, float u, float v);

/**
 * @brief Convert pixel coordinates to UV coordinates
 * @param camera The camera
 * @param pixel_x Pixel x coordinate
 * @param pixel_y Pixel y coordinate
 * @param u Output horizontal UV coordinate
 * @param v Output vertical UV coordinate
 */
void camera_pixel_to_uv(const Camera *camera, int pixel_x, int pixel_y, 
                       float *u, float *v);

/**
 * @brief Print camera information (for debugging)
 * @param camera The camera to print
 */
void camera_print(const Camera *camera);

#endif // CAMERA_H 