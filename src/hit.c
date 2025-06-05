/**
 * @file hit.c
 * @brief Hit detection interface implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "hit.h"
#include <stdio.h>

Hittable hittable_create(void *data, HitFunction hit_func) {
    Hittable hittable;
    hittable.data = data;
    hittable.hit_func = hit_func;
    return hittable;
}

bool hittable_hit(const Hittable *object, const Ray *ray, 
                  float t_min, float t_max, HitRecord *hit_rec) {
    if (object->hit_func) {
        return object->hit_func(object, ray, t_min, t_max, hit_rec);
    }
    return false;
}

void hit_record_set_face_normal(HitRecord *hit_rec, const Ray *ray, Vec3 outward_normal) {
    // Determine if we're hitting the front face or back face
    hit_rec->front_face = vec3_dot(ray->direction, outward_normal) < 0;
    
    // Always point normal against ray direction
    if (hit_rec->front_face) {
        hit_rec->normal = outward_normal;
    } else {
        hit_rec->normal = vec3_negate(outward_normal);
    }
}

void hit_record_print(const HitRecord *hit_rec) {
    printf("HitRecord(\n");
    printf("  point: ");
    vec3_print(hit_rec->point);
    printf("  normal: ");
    vec3_print(hit_rec->normal);
    printf("  t: %.6f\n", hit_rec->t);
    printf("  front_face: %s\n", hit_rec->front_face ? "true" : "false");
    printf(")\n");
}