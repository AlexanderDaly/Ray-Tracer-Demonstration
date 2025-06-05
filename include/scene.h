/**
 * @file scene.h
 * @brief Scene management for ray tracing
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hit.h"

#define MAX_OBJECTS 32

/**
 * @brief Point light structure
 */
typedef struct {
    Vec3 position;     ///< Light position
    Color color;       ///< Light color/intensity
    float intensity;   ///< Light intensity multiplier
} PointLight;

/**
 * @brief Scene containing objects and lighting
 */
typedef struct {
    Hittable objects[MAX_OBJECTS];  ///< Array of hittable objects
    int object_count;               ///< Number of objects in scene
    PointLight lights[8];           ///< Array of point lights
    int light_count;                ///< Number of lights in scene
    Color background_color;         ///< Background color
} Scene;

/**
 * @brief Create an empty scene
 * @param background_color Background color for rays that hit nothing
 * @return Empty scene
 */
Scene scene_create(Color background_color);

/**
 * @brief Add an object to the scene
 * @param scene Scene to add to
 * @param object Hittable object to add
 * @return true if added successfully, false if scene is full
 */
bool scene_add_object(Scene *scene, Hittable object);

/**
 * @brief Add a point light to the scene
 * @param scene Scene to add to
 * @param light Point light to add
 * @return true if added successfully, false if scene is full
 */
bool scene_add_light(Scene *scene, PointLight light);

/**
 * @brief Test ray intersection with all objects in scene
 * @param scene Scene to test
 * @param ray Ray to test
 * @param t_min Minimum ray parameter
 * @param t_max Maximum ray parameter
 * @param hit_rec Output hit record
 * @return true if any object was hit
 */
bool scene_hit(const Scene *scene, const Ray *ray, float t_min, float t_max, HitRecord *hit_rec);

/**
 * @brief Calculate color for a ray in the scene
 * @param scene Scene to render
 * @param ray Ray to trace
 * @param depth Recursion depth (for reflections)
 * @return Final color for the ray
 */
Color scene_ray_color(const Scene *scene, const Ray *ray, int depth);

/**
 * @brief Simple Lambertian shading calculation
 * @param scene Scene with lights
 * @param hit_rec Hit information
 * @param material_color Base material color
 * @return Shaded color
 */
Color scene_shade_lambertian(const Scene *scene, const HitRecord *hit_rec, Color material_color);

/**
 * @brief Print scene information
 * @param scene Scene to print
 */
void scene_print(const Scene *scene);

#endif // SCENE_H