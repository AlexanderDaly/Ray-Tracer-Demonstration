/**
 * @file render.h
 * @brief Rendering pipeline for ray tracer
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include "scene.h"
#include "color.h"
#include <stdio.h>

/**
 * @brief Render settings
 */
typedef struct {
    int samples_per_pixel;  ///< Number of samples per pixel (for anti-aliasing)
    int max_depth;          ///< Maximum ray bounce depth
    bool show_progress;     ///< Whether to show progress during render
} RenderSettings;

/**
 * @brief Create default render settings
 * @return Default settings for basic rendering
 */
RenderSettings render_settings_default(void);

/**
 * @brief Render a scene to a PPM file
 * @param scene Scene to render
 * @param camera Camera configuration
 * @param settings Render settings
 * @param output_file File to write PPM data to
 * @return true if render completed successfully
 */
bool render_scene_to_file(const Scene *scene, const Camera *camera, 
                         const RenderSettings *settings, FILE *output_file);

/**
 * @brief Render a single pixel
 * @param scene Scene to render
 * @param camera Camera configuration
 * @param pixel_x Pixel X coordinate
 * @param pixel_y Pixel Y coordinate
 * @param settings Render settings
 * @return Color for the pixel
 */
Color render_pixel(const Scene *scene, const Camera *camera, 
                  int pixel_x, int pixel_y, const RenderSettings *settings);

/**
 * @brief Get random float in range [0, 1)
 * Simple linear congruential generator for sampling
 * @return Random float
 */
float random_float(void);

/**
 * @brief Set random seed
 * @param seed Seed value
 */
void set_random_seed(unsigned int seed);

#endif // RENDER_H