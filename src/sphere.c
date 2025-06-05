/**
 * @file sphere.c
 * @brief Sphere geometry and intersection implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "sphere.h"
#include <stdio.h>
#include <math.h>

Sphere sphere_create(Vec3 center, float radius, Color color) {
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius > 0 ? radius : 1.0f;  // Ensure positive radius
    sphere.color = color;
    return sphere;
}

bool sphere_hit(const Hittable *hittable, const Ray *ray, 
                float t_min, float t_max, HitRecord *hit_rec) {
    const Sphere *sphere = (const Sphere *)hittable->data;
    
    // Ray-sphere intersection using quadratic formula
    // Ray: P(t) = origin + t * direction
    // Sphere: |P - center|² = radius²
    // Substituting: |origin + t*direction - center|² = radius²
    
    Vec3 oc = vec3_sub(ray->origin, sphere->center);
    float a = vec3_length_squared(ray->direction);
    float half_b = vec3_dot(oc, ray->direction);
    float c = vec3_length_squared(oc) - sphere->radius * sphere->radius;
    
    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return false;  // No intersection
    }
    
    float sqrtd = sqrtf(discriminant);
    
    // Find the nearest root that lies in the acceptable range
    float root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) {
            return false;  // Both roots outside range
        }
    }
    
    // Fill hit record
    hit_rec->t = root;
    hit_rec->point = ray_at(ray, root);
    Vec3 outward_normal = vec3_div(vec3_sub(hit_rec->point, sphere->center), sphere->radius);
    hit_record_set_face_normal(hit_rec, ray, outward_normal);
    
    return true;
}

Hittable sphere_to_hittable(Sphere *sphere) {
    return hittable_create(sphere, sphere_hit);
}

Vec3 sphere_normal_at(const Sphere *sphere, Vec3 point) {
    Vec3 normal = vec3_sub(point, sphere->center);
    return vec3_normalize(normal);
}

bool sphere_contains_point(const Sphere *sphere, Vec3 point) {
    Vec3 to_point = vec3_sub(point, sphere->center);
    float distance_squared = vec3_length_squared(to_point);
    return distance_squared <= (sphere->radius * sphere->radius);
}

void sphere_print(const Sphere *sphere) {
    printf("Sphere(\n");
    printf("  center: ");
    vec3_print(sphere->center);
    printf("  radius: %.3f\n", sphere->radius);
    printf("  color: ");
    color_print(sphere->color);
    printf(")\n");
}