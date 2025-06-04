/**
 * @file test_vec3.c
 * @brief Unit tests for Vec3 operations  
 */

#include "unity/unity.h"
#include "vec3.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_vec3_basic(void) {
    Vec3 v = vec3_create(1.0f, 2.0f, 3.0f);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 1.0f, v.x);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 2.0f, v.y);
    TEST_ASSERT_FLOAT_WITHIN(1e-6f, 3.0f, v.z);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_vec3_basic);
    return UNITY_END();
} 