/**
 * @file ray.c
 * @brief Ray implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "ray.h"
#include <stdio.h>

Ray ray_create(Vec3 origin, Vec3 direction) {
    Ray ray;
    ray.origin = origin;
    ray.direction = vec3_normalize(direction);
    return ray;
}

Vec3 ray_at(const Ray *ray, float t) {
    Vec3 scaled_dir = vec3_scale(ray->direction, t);
    return vec3_add(ray->origin, scaled_dir);
}

bool ray_is_normalized(const Ray *ray) {
    float length = vec3_length(ray->direction);
    return vec3_equal(vec3_create(length, 0, 0), vec3_create(1.0f, 0, 0), 1e-6f);
}

void ray_print(const Ray *ray) {
    printf("Ray {\n");
    printf("  origin: ");
    vec3_print(ray->origin);
    printf("  direction: ");
    vec3_print(ray->direction);
    printf("}\n");
}