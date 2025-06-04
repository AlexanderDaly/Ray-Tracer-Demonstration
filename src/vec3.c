/**
 * @file vec3.c
 * @brief 3D vector mathematics implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "vec3.h"
#include <stdio.h>
#include <math.h>

#define EPSILON 1e-6f

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 vec3_scale(Vec3 v, float scalar) {
    return (Vec3){v.x * scalar, v.y * scalar, v.z * scalar};
}

Vec3 vec3_div(Vec3 v, float scalar) {
    float inv = 1.0f / scalar;
    return vec3_scale(v, inv);
}

Vec3 vec3_negate(Vec3 v) {
    return (Vec3){-v.x, -v.y, -v.z};
}

float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    return (Vec3){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vec3_length_squared(Vec3 v) {
    return vec3_dot(v, v);
}

float vec3_length(Vec3 v) {
    return sqrtf(vec3_length_squared(v));
}

Vec3 vec3_normalize(Vec3 v) {
    float len = vec3_length(v);
    if (len < EPSILON) {
        return vec3_zero();
    }
    return vec3_div(v, len);
}

bool vec3_equal(Vec3 a, Vec3 b, float epsilon) {
    return fabsf(a.x - b.x) < epsilon &&
           fabsf(a.y - b.y) < epsilon &&
           fabsf(a.z - b.z) < epsilon;
}

bool vec3_near_zero(Vec3 v) {
    return vec3_equal(v, vec3_zero(), EPSILON);
}

Vec3 vec3_reflect(Vec3 v, Vec3 n) {
    // r = v - 2 * dot(v, n) * n
    float dot_vn = vec3_dot(v, n);
    Vec3 scaled_n = vec3_scale(n, 2.0f * dot_vn);
    return vec3_sub(v, scaled_n);
}

Vec3 vec3_lerp(Vec3 a, Vec3 b, float t) {
    Vec3 diff = vec3_sub(b, a);
    Vec3 scaled = vec3_scale(diff, t);
    return vec3_add(a, scaled);
}

void vec3_print(Vec3 v) {
    printf("Vec3(%.3f, %.3f, %.3f)\n", v.x, v.y, v.z);
} 