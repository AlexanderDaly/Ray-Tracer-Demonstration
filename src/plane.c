/**
 * @file plane.c
 * @brief Infinite plane geometry implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "plane.h"
#include <stdio.h>
#include <math.h>

#define EPSILON 1e-6f

Plane plane_create(Vec3 point, Vec3 normal, Color color) {
    Plane plane;
    plane.point = point;
    plane.normal = vec3_normalize(normal);
    plane.color = color;
    return plane;
}

Plane plane_create_from_points(Vec3 p1, Vec3 p2, Vec3 p3, Color color) {
    Vec3 v1 = vec3_sub(p2, p1);
    Vec3 v2 = vec3_sub(p3, p1);
    Vec3 normal = vec3_cross(v1, v2);
    return plane_create(p1, normal, color);
}

Plane plane_create_xz(float y, Color color) {
    Vec3 point = vec3_create(0.0f, y, 0.0f);
    Vec3 normal = vec3_create(0.0f, 1.0f, 0.0f);
    return plane_create(point, normal, color);
}

bool plane_hit(const Hittable *hittable, const Ray *ray, 
               float t_min, float t_max, HitRecord *hit_rec) {
    const Plane *plane = (const Plane *)hittable->data;
    
    // Ray-plane intersection
    // Plane equation: dot(normal, point - plane_point) = 0
    // Ray: P(t) = origin + t * direction
    // Substituting: dot(normal, origin + t*direction - plane_point) = 0
    
    float denom = vec3_dot(plane->normal, ray->direction);
    
    // Check if ray is parallel to plane
    if (fabsf(denom) < EPSILON) {
        return false;
    }
    
    Vec3 to_plane = vec3_sub(plane->point, ray->origin);
    float t = vec3_dot(to_plane, plane->normal) / denom;
    
    if (t < t_min || t > t_max) {
        return false;
    }
    
    // Fill hit record
    hit_rec->t = t;
    hit_rec->point = ray_at(ray, t);
    hit_record_set_face_normal(hit_rec, ray, plane->normal);
    
    return true;
}

Hittable plane_to_hittable(Plane *plane) {
    return hittable_create(plane, plane_hit);
}

float plane_distance_to_point(const Plane *plane, Vec3 point) {
    Vec3 to_point = vec3_sub(point, plane->point);
    return vec3_dot(to_point, plane->normal);
}

bool plane_point_in_front(const Plane *plane, Vec3 point) {
    return plane_distance_to_point(plane, point) > 0.0f;
}

void plane_print(const Plane *plane) {
    printf("Plane {\n");
    printf("  point: ");
    vec3_print(plane->point);
    printf("  normal: ");
    vec3_print(plane->normal);
    printf("  color: ");
    color_print(plane->color);
    printf("}\n");
}