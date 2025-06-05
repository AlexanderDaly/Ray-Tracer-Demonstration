/**
 * @file render.h
 * @brief Core rendering system with scene management
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef RENDER_H
#define RENDER_H

#include "camera.h"
#include "hit.h"
#include "color.h"
#include "vec3.h"
#include <stdio.h>

/**
 * @brief Clear all objects from the scene
 */
void scene_clear(void);

/**
 * @brief Add an object to the scene
 * @param object Hittable object to add
 */
void scene_add_object(Hittable object);

/**
 * @brief Set the scene lighting
 * @param position Position of the point light
 * @param color Color/intensity of the light
 */
void scene_set_light(Vec3 position, Color color);

/**
 * @brief Set ambient lighting
 * @param color Ambient light color
 */
void scene_set_ambient(Color color);

/**
 * @brief Render the scene to a PPM file
 * @param camera Camera to render from
 * @param output File stream to write PPM data to
 */
void render_scene(const Camera *camera, FILE *output);

#endif // RENDER_H