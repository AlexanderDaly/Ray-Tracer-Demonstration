/**
 * @file color.h
 * @brief RGB color representation and operations
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <stdint.h>

/**
 * @brief RGB color (same layout as Vec3)
 * Components: r, g, b in range [0.0, 1.0]
 */
typedef Vec3 Color;

/**
 * @brief Create color from RGB components
 */
static inline Color color_create(float r, float g, float b) {
    return (Color){r, g, b};
}

/**
 * @brief Black color (0, 0, 0)
 */
static inline Color color_black(void) {
    return (Color){0.0f, 0.0f, 0.0f};
}

/**
 * @brief White color (1, 1, 1)
 */
static inline Color color_white(void) {
    return (Color){1.0f, 1.0f, 1.0f};
}

/**
 * @brief Red color (1, 0, 0)
 */
static inline Color color_red(void) {
    return (Color){1.0f, 0.0f, 0.0f};
}

/**
 * @brief Green color (0, 1, 0)
 */
static inline Color color_green(void) {
    return (Color){0.0f, 1.0f, 0.0f};
}

/**
 * @brief Blue color (0, 0, 1)
 */
static inline Color color_blue(void) {
    return (Color){0.0f, 0.0f, 1.0f};
}

/**
 * @brief Add two colors (component-wise)
 */
static inline Color color_add(Color a, Color b) {
    return vec3_add(a, b);
}

/**
 * @brief Multiply two colors (component-wise)
 */
Color color_multiply(Color a, Color b);

/**
 * @brief Scale color by scalar
 */
static inline Color color_scale(Color c, float scalar) {
    return vec3_scale(c, scalar);
}

/**
 * @brief Clamp color components to [0.0, 1.0]
 */
Color color_clamp(Color c);

/**
 * @brief Apply gamma correction (gamma = 2.0 by default)
 */
Color color_gamma_correct(Color c, float gamma);

/**
 * @brief Convert color to 8-bit RGB values
 * @param c Color with components in [0.0, 1.0]
 * @param r Output red component [0, 255]
 * @param g Output green component [0, 255]
 * @param b Output blue component [0, 255]
 */
void color_to_u8(Color c, uint8_t *r, uint8_t *g, uint8_t *b);

/**
 * @brief Convert color to 8-bit packed RGB (0xRRGGBB)
 */
uint32_t color_to_rgb24(Color c);

/**
 * @brief Linear interpolation between two colors
 */
static inline Color color_lerp(Color a, Color b, float t) {
    return vec3_lerp(a, b, t);
}

/**
 * @brief Print color values (for debugging)
 */
void color_print(Color c);

#endif // COLOR_H 