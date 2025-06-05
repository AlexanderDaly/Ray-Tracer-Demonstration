/**
 * @file test_vec3.c
 * @brief Unit tests for Vec3 operations  
 */

#include "unity/unity.h"
#include "vec3.h"

void test_vec3_basic(void) {
    Vec3 v = vec3_create(1.0f, 2.0f, 3.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 1.0f, v.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 2.0f, v.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 3.0f, v.z);
}

void test_vec3_add(void) {
    Vec3 a = vec3_create(1.0f, 2.0f, 3.0f);
    Vec3 b = vec3_create(4.0f, 5.0f, 6.0f);
    Vec3 result = vec3_add(a, b);
    
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 5.0f, result.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 7.0f, result.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 9.0f, result.z);
}

void test_vec3_dot(void) {
    Vec3 a = vec3_create(1.0f, 2.0f, 3.0f);
    Vec3 b = vec3_create(4.0f, 5.0f, 6.0f);
    float result = vec3_dot(a, b);
    
    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 32.0f, result);
}

void run_vec3_tests(void) {
    RUN_TEST(test_vec3_basic);
    RUN_TEST(test_vec3_add);
    RUN_TEST(test_vec3_dot);
} 