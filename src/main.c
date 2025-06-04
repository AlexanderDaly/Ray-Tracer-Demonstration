/**
 * @file main.c
 * @brief Ray Tracer Demonstration - Main entry point
 * @author Morpheus (Project Manager & Architect)
 * @date June 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

// Include all our headers
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "camera.h"
#include "hit.h"
#include "sphere.h"
#include "plane.h"

/**
 * @brief Print usage information
 */
static void print_usage(const char *program_name) {
    printf("Ray Tracer Demonstration v0.1\n");
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("\nOptions:\n");
    printf("  -w, --width WIDTH    Image width in pixels (default: 400)\n");
    printf("  -h, --height HEIGHT  Image height in pixels (default: 225)\n");
    printf("  -o, --output FILE    Output PPM file (default: output.ppm)\n");
    printf("  --help               Show this help message\n");
    printf("\nExample:\n");
    printf("  %s -w 800 -h 600 -o render.ppm\n", program_name);
}

/**
 * @brief Simple PPM writer
 */
static void write_ppm_header(FILE *file, int width, int height) {
    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "255\n");
}

/**
 * @brief Write a color to PPM format
 */
static void write_ppm_color(FILE *file, Color color) {
    uint8_t r, g, b;
    color_to_u8(color, &r, &g, &b);
    fprintf(file, "%d %d %d\n", r, g, b);
}

/**
 * @brief Simple test render - gradient background
 */
static void render_test_scene(FILE *output, int width, int height) {
    write_ppm_header(output, width, height);
    
    for (int j = height - 1; j >= 0; j--) {
        fprintf(stderr, "\rScanlines remaining: %d ", j);
        fflush(stderr);
        
        for (int i = 0; i < width; i++) {
            float u = (float)i / (float)(width - 1);
            float v = (float)j / (float)(height - 1);
            
            // Simple gradient: blue to white
            Color pixel_color = color_create(u, v, 0.5f);
            write_ppm_color(output, pixel_color);
        }
    }
    
    fprintf(stderr, "\nDone.\n");
}

/**
 * @brief Main entry point
 */
int main(int argc, char *argv[]) {
    // Default parameters
    int image_width = 400;
    int image_height = 225;
    const char *output_filename = "output.ppm";
    
    // Command line option structure
    static struct option long_options[] = {
        {"width",  required_argument, 0, 'w'},
        {"height", required_argument, 0, 'h'},
        {"output", required_argument, 0, 'o'},
        {"help",   no_argument,       0, 0},
        {0, 0, 0, 0}
    };
    
    // Parse command line arguments
    int option_index = 0;
    int c;
    
    while ((c = getopt_long(argc, argv, "w:h:o:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'w':
                image_width = atoi(optarg);
                if (image_width <= 0) {
                    fprintf(stderr, "Error: Width must be positive\n");
                    return 1;
                }
                break;
                
            case 'h':
                image_height = atoi(optarg);
                if (image_height <= 0) {
                    fprintf(stderr, "Error: Height must be positive\n");
                    return 1;
                }
                break;
                
            case 'o':
                output_filename = optarg;
                break;
                
            case 0:
                if (strcmp(long_options[option_index].name, "help") == 0) {
                    print_usage(argv[0]);
                    return 0;
                }
                break;
                
            case '?':
                print_usage(argv[0]);
                return 1;
                
            default:
                break;
        }
    }
    
    // Validate parameters
    if (image_width > 4096 || image_height > 4096) {
        fprintf(stderr, "Warning: Large image size may be slow\n");
    }
    
    printf("Ray Tracer Demonstration v0.1\n");
    printf("Rendering %dx%d image to '%s'\n", image_width, image_height, output_filename);
    
    // Open output file
    FILE *output = fopen(output_filename, "w");
    if (!output) {
        fprintf(stderr, "Error: Could not open output file '%s'\n", output_filename);
        return 1;
    }
    
    // Render the scene (currently just a test pattern)
    render_test_scene(output, image_width, image_height);
    
    fclose(output);
    
    printf("Render complete! Output written to '%s'\n", output_filename);
    printf("\nTo view the image:\n");
    printf("  - On macOS: open %s\n", output_filename);
    printf("  - On Linux: display %s  (ImageMagick)\n", output_filename);
    printf("  - On Windows: Use any image viewer that supports PPM\n");
    
    return 0;
} 