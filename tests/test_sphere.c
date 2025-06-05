/**
 * @file test_sphere.c
 * @brief Unit tests for sphere intersection
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "unity/unity.h"
#include "sphere.h"
#include "ray.h"

void test_sphere_creation(void) {
    Sphere sphere = sphere_create(
        vec3_create(0.0f, 0.0f, 0.0f),
        1.0f,
        color_red()
    );
    
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, sphere.center.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, sphere.center.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.0f, sphere.center.z);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 1.0f, sphere.radius);
}

void test_sphere_ray_hit(void) {
    // Create sphere at origin with radius 1
    Sphere sphere = sphere_create(
        vec3_create(0.0f, 0.0f, -1.0f),
        0.5f,
        color_red()
    );
    
    Hittable hittable = sphere_to_hittable(&sphere);
    
    // Ray from origin pointing towards sphere
    Ray ray = ray_create(
        vec3_create(0.0f, 0.0f, 0.0f),
        vec3_create(0.0f, 0.0f, -1.0f)
    );
    
    HitRecord hit_rec;
    bool hit = hittable_hit(&hittable, &ray, 0.0f, 10.0f, &hit_rec);
    
    TEST_ASSERT_TRUE(hit);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 0.5f, hit_rec.t); // Should hit at t=0.5
    TEST_ASSERT_TRUE(hit_rec.front_face);
}

void test_sphere_ray_miss(void) {
    // Create sphere at origin with radius 1
    Sphere sphere = sphere_create(
        vec3_create(0.0f, 0.0f, -1.0f),
        0.5f,
        color_red()
    );
    
    Hittable hittable = sphere_to_hittable(&sphere);
    
    // Ray that misses the sphere
    Ray ray = ray_create(
        vec3_create(0.0f, 0.0f, 0.0f),
        vec3_create(1.0f, 0.0f, 0.0f)
    );
    
    HitRecord hit_rec;
    bool hit = hittable_hit(&hittable, &ray, 0.0f, 10.0f, &hit_rec);
    
    TEST_ASSERT_FALSE(hit);
}

void run_sphere_tests(void) {
    RUN_TEST(test_sphere_creation);
    RUN_TEST(test_sphere_ray_hit);
    RUN_TEST(test_sphere_ray_miss);
}