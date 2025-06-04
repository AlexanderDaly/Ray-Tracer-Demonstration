/**
 * @file plane.h
 * @brief Infinite plane geometry implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef PLANE_H
#define PLANE_H

#include "vec3.h"
#include "ray.h"
#include "hit.h"
#include "color.h"

/**
 * @brief Infinite plane structure
 */
typedef struct {
    Vec3 point;    ///< A point on the plane
    Vec3 normal;   ///< Unit normal vector of the plane
    Color color;   ///< Material color of plane
} Plane;

/**
 * @brief Create a plane from point and normal
 * @param point Any point on the plane
 * @param normal Normal vector (will be normalized)
 * @param color Material color
 * @return Plane structure
 */
Plane plane_create(Vec3 point, Vec3 normal, Color color);

/**
 * @brief Create a plane from three points
 * @param p1 First point
 * @param p2 Second point
 * @param p3 Third point
 * @param color Material color
 * @return Plane structure
 */
Plane plane_create_from_points(Vec3 p1, Vec3 p2, Vec3 p3, Color color);

/**
 * @brief Create an XZ plane (horizontal plane at given Y)
 * @param y Y coordinate of the plane
 * @param color Material color
 * @return Plane structure
 */
Plane plane_create_xz(float y, Color color);

/**
 * @brief Test ray-plane intersection
 * @param plane Pointer to plane data (cast from void*)
 * @param ray Ray to test
 * @param t_min Minimum ray parameter
 * @param t_max Maximum ray parameter
 * @param hit_rec Output hit record
 * @return true if intersection found
 */
bool plane_hit(const Hittable *plane, const Ray *ray, 
               float t_min, float t_max, HitRecord *hit_rec);

/**
 * @brief Create a hittable plane object
 * @param plane Pointer to plane structure
 * @return Hittable object wrapping the plane
 */
Hittable plane_to_hittable(Plane *plane);

/**
 * @brief Calculate distance from point to plane
 * @param plane The plane
 * @param point Point to test
 * @return Signed distance (positive = in front, negative = behind)
 */
float plane_distance_to_point(const Plane *plane, Vec3 point);

/**
 * @brief Check which side of plane a point is on
 * @param plane The plane
 * @param point Point to test
 * @return true if point is on front side (normal direction)
 */
bool plane_point_in_front(const Plane *plane, Vec3 point);

/**
 * @brief Print plane information (for debugging)
 * @param plane The plane to print
 */
void plane_print(const Plane *plane);

#endif // PLANE_H 