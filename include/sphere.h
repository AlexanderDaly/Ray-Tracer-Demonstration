/**
 * @file sphere.h
 * @brief Sphere geometry implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "color.h"

/**
 * @brief Sphere structure
 */
typedef struct {
    Vec3 center;   ///< Center point of sphere
    float radius;  ///< Radius of sphere
    Color color;   ///< Material color of sphere
} Sphere;

/**
 * @brief Create a sphere
 * @param center Center point
 * @param radius Radius (must be positive)
 * @param color Material color
 * @return Sphere structure
 */
Sphere sphere_create(Vec3 center, float radius, Color color);

/**
 * @brief Test ray-sphere intersection
 * @param sphere Pointer to sphere data (cast from void*)
 * @param ray Ray to test
 * @param t_min Minimum ray parameter
 * @param t_max Maximum ray parameter
 * @param hit_rec Output hit record
 * @return true if intersection found
 */
bool sphere_hit(const Hittable *sphere, const Ray *ray, 
                float t_min, float t_max, HitRecord *hit_rec);

/**
 * @brief Create a hittable sphere object
 * @param sphere Pointer to sphere structure
 * @return Hittable object wrapping the sphere
 */
Hittable sphere_to_hittable(Sphere *sphere);

/**
 * @brief Get surface normal at a point on sphere
 * @param sphere The sphere
 * @param point Point on sphere surface
 * @return Outward-facing unit normal
 */
Vec3 sphere_normal_at(const Sphere *sphere, Vec3 point);

/**
 * @brief Check if point is inside sphere
 * @param sphere The sphere
 * @param point Point to test
 * @return true if point is inside sphere
 */
bool sphere_contains_point(const Sphere *sphere, Vec3 point);

/**
 * @brief Print sphere information (for debugging)
 * @param sphere The sphere to print
 */
void sphere_print(const Sphere *sphere);

#endif // SPHERE_H 