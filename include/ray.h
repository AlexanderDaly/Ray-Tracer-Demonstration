/**
 * @file ray.h
 * @brief Ray representation for ray tracing
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

/**
 * @brief Ray structure with origin and direction
 */
typedef struct {
    Vec3 origin;    ///< Ray origin point
    Vec3 direction; ///< Ray direction (should be normalized)
} Ray;

/**
 * @brief Create a ray from origin and direction
 * @param origin Starting point of the ray
 * @param direction Direction vector (will be normalized)
 * @return New ray structure
 */
Ray ray_create(Vec3 origin, Vec3 direction);

/**
 * @brief Get point along ray at parameter t
 * @param ray The ray
 * @param t Parameter along ray (origin + t * direction)
 * @return Point at parameter t
 */
Vec3 ray_at(const Ray *ray, float t);

/**
 * @brief Check if ray direction is normalized
 * @param ray The ray to check
 * @return true if direction is approximately unit length
 */
bool ray_is_normalized(const Ray *ray);

/**
 * @brief Print ray information (for debugging)
 * @param ray The ray to print
 */
void ray_print(const Ray *ray);

#endif // RAY_H 