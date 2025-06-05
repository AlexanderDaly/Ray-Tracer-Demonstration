/**
 * @file sphere.c
 * @brief Sphere geometry implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "sphere.h"
#include <stdio.h>
#include <math.h>

Sphere sphere_create(Vec3 center, float radius, Color color) {
    Sphere sphere;
    sphere.center = center;
    sphere.radius = radius;
    sphere.color = color;
    return sphere;
}

bool sphere_hit(const Hittable *hittable, const Ray *ray, 
                float t_min, float t_max, HitRecord *hit_rec) {
    const Sphere *sphere = (const Sphere *)hittable->data;
    
    // Ray-sphere intersection using quadratic formula
    Vec3 oc = vec3_sub(ray->origin, sphere->center);
    float a = vec3_dot(ray->direction, ray->direction);
    float b = 2.0f * vec3_dot(oc, ray->direction);
    float c = vec3_dot(oc, oc) - sphere->radius * sphere->radius;
    
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f) {
        return false; // No intersection
    }
    
    // Find the nearest intersection
    float sqrt_discriminant = sqrtf(discriminant);
    float root = (-b - sqrt_discriminant) / (2.0f * a);
    
    if (root < t_min || root > t_max) {
        root = (-b + sqrt_discriminant) / (2.0f * a);
        if (root < t_min || root > t_max) {
            return false;
        }
    }
    
    // Fill hit record
    hit_rec->t = root;
    hit_rec->point = ray_at(ray, hit_rec->t);
    Vec3 outward_normal = vec3_div(vec3_sub(hit_rec->point, sphere->center), sphere->radius);
    hit_record_set_face_normal(hit_rec, ray, outward_normal);
    
    return true;
}

Hittable sphere_to_hittable(Sphere *sphere) {
    return hittable_create(sphere, sphere_hit);
}

Vec3 sphere_normal_at(const Sphere *sphere, Vec3 point) {
    return vec3_normalize(vec3_sub(point, sphere->center));
}

bool sphere_contains_point(const Sphere *sphere, Vec3 point) {
    Vec3 to_point = vec3_sub(point, sphere->center);
    return vec3_length_squared(to_point) <= sphere->radius * sphere->radius;
}

void sphere_print(const Sphere *sphere) {
    printf("Sphere {\n");
    printf("  center: ");
    vec3_print(sphere->center);
    printf("  radius: %.3f\n", sphere->radius);
    printf("  color: ");
    color_print(sphere->color);
    printf("}\n");
}