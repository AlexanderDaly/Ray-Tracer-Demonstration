/**
 * @file test_runner.c
 * @brief Main test runner for all test suites
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "unity/unity.h"

// External test functions
extern void run_vec3_tests(void);
extern void run_sphere_tests(void);

void setUp(void) {
    // Global setup
}

void tearDown(void) {
    // Global teardown
}

int main(void) {
    UNITY_BEGIN();
    
    // Run all test suites
    run_vec3_tests();
    run_sphere_tests();
    
    return UNITY_END();
}