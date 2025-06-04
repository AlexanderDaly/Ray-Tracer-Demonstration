/**
 * @file hit.h
 * @brief Hit detection interface for ray-object intersection
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef HIT_H
#define HIT_H

#include "vec3.h"
#include "ray.h"
#include <stdbool.h>

/**
 * @brief Hit record containing intersection information
 */
typedef struct {
    Vec3 point;      ///< Intersection point
    Vec3 normal;     ///< Surface normal at intersection (unit vector)
    float t;         ///< Ray parameter at intersection
    bool front_face; ///< True if ray hits front face of surface
} HitRecord;

/**
 * @brief Forward declaration for hittable object
 */
typedef struct Hittable Hittable;

/**
 * @brief Function pointer type for hit testing
 * @param object Pointer to the hittable object
 * @param ray Ray to test for intersection
 * @param t_min Minimum ray parameter to consider
 * @param t_max Maximum ray parameter to consider
 * @param hit_rec Output hit record (only valid if function returns true)
 * @return true if intersection found, false otherwise
 */
typedef bool (*HitFunction)(const Hittable *object, const Ray *ray, 
                           float t_min, float t_max, HitRecord *hit_rec);

/**
 * @brief Hittable object interface
 */
struct Hittable {
    void *data;           ///< Pointer to object-specific data
    HitFunction hit_func; ///< Function to test ray intersection
};

/**
 * @brief Create a hittable object
 * @param data Pointer to object-specific data
 * @param hit_func Function to handle ray intersection
 * @return Hittable object
 */
Hittable hittable_create(void *data, HitFunction hit_func);

/**
 * @brief Test ray intersection with hittable object
 * @param object The hittable object
 * @param ray Ray to test
 * @param t_min Minimum ray parameter
 * @param t_max Maximum ray parameter
 * @param hit_rec Output hit record
 * @return true if intersection found
 */
bool hittable_hit(const Hittable *object, const Ray *ray, 
                  float t_min, float t_max, HitRecord *hit_rec);

/**
 * @brief Set face normal based on ray direction
 * Determines if ray hits front or back face and sets normal accordingly
 * @param hit_rec Hit record to modify
 * @param ray The ray that caused the hit
 * @param outward_normal Surface normal pointing outward from object
 */
void hit_record_set_face_normal(HitRecord *hit_rec, const Ray *ray, Vec3 outward_normal);

/**
 * @brief Print hit record information (for debugging)
 * @param hit_rec Hit record to print
 */
void hit_record_print(const HitRecord *hit_rec);

#endif // HIT_H 