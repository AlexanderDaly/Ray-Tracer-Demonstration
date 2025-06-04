/**
 * @file vec3.h
 * @brief 3D vector mathematics for ray tracer
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef VEC3_H
#define VEC3_H

#include <stdbool.h>
#include <math.h>

/**
 * @brief 3D vector structure
 */
typedef struct {
    float x, y, z;
} Vec3;

/**
 * @brief Create a vector from components
 */
static inline Vec3 vec3_create(float x, float y, float z) {
    return (Vec3){x, y, z};
}

/**
 * @brief Zero vector
 */
static inline Vec3 vec3_zero(void) {
    return (Vec3){0.0f, 0.0f, 0.0f};
}

/**
 * @brief Unit vector along X axis
 */
static inline Vec3 vec3_unit_x(void) {
    return (Vec3){1.0f, 0.0f, 0.0f};
}

/**
 * @brief Unit vector along Y axis
 */
static inline Vec3 vec3_unit_y(void) {
    return (Vec3){0.0f, 1.0f, 0.0f};
}

/**
 * @brief Unit vector along Z axis
 */
static inline Vec3 vec3_unit_z(void) {
    return (Vec3){0.0f, 0.0f, 1.0f};
}

/**
 * @brief Add two vectors
 */
Vec3 vec3_add(Vec3 a, Vec3 b);

/**
 * @brief Subtract two vectors
 */
Vec3 vec3_sub(Vec3 a, Vec3 b);

/**
 * @brief Multiply vector by scalar
 */
Vec3 vec3_scale(Vec3 v, float scalar);

/**
 * @brief Divide vector by scalar
 */
Vec3 vec3_div(Vec3 v, float scalar);

/**
 * @brief Negate vector
 */
Vec3 vec3_negate(Vec3 v);

/**
 * @brief Dot product of two vectors
 */
float vec3_dot(Vec3 a, Vec3 b);

/**
 * @brief Cross product of two vectors
 */
Vec3 vec3_cross(Vec3 a, Vec3 b);

/**
 * @brief Length (magnitude) of vector
 */
float vec3_length(Vec3 v);

/**
 * @brief Squared length of vector (avoids sqrt)
 */
float vec3_length_squared(Vec3 v);

/**
 * @brief Normalize vector to unit length
 */
Vec3 vec3_normalize(Vec3 v);

/**
 * @brief Check if vector is approximately equal to another
 */
bool vec3_equal(Vec3 a, Vec3 b, float epsilon);

/**
 * @brief Check if vector is near zero
 */
bool vec3_near_zero(Vec3 v);

/**
 * @brief Reflect vector v across normal n
 */
Vec3 vec3_reflect(Vec3 v, Vec3 n);

/**
 * @brief Linear interpolation between two vectors
 */
Vec3 vec3_lerp(Vec3 a, Vec3 b, float t);

/**
 * @brief Print vector to stdout (for debugging)
 */
void vec3_print(Vec3 v);

#endif // VEC3_H 