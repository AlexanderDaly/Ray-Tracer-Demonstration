/**
 * @file color.c
 * @brief RGB color operations implementation
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include "color.h"
#include <stdio.h>
#include <math.h>

Color color_multiply(Color a, Color b) {
    return color_create(a.x * b.x, a.y * b.y, a.z * b.z);
}

Color color_clamp(Color c) {
    float r = c.x < 0.0f ? 0.0f : (c.x > 1.0f ? 1.0f : c.x);
    float g = c.y < 0.0f ? 0.0f : (c.y > 1.0f ? 1.0f : c.y);
    float b = c.z < 0.0f ? 0.0f : (c.z > 1.0f ? 1.0f : c.z);
    return color_create(r, g, b);
}

Color color_gamma_correct(Color c, float gamma) {
    return color_create(
        powf(c.x, 1.0f / gamma),
        powf(c.y, 1.0f / gamma),
        powf(c.z, 1.0f / gamma)
    );
}

void color_to_u8(Color c, uint8_t *r, uint8_t *g, uint8_t *b) {
    Color clamped = color_clamp(c);
    *r = (uint8_t)(255.0f * clamped.x);
    *g = (uint8_t)(255.0f * clamped.y);
    *b = (uint8_t)(255.0f * clamped.z);
}

uint32_t color_to_rgb24(Color c) {
    uint8_t r, g, b;
    color_to_u8(c, &r, &g, &b);
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}

void color_print(Color c) {
    printf("Color(%.3f, %.3f, %.3f)\n", c.x, c.y, c.z);
} 