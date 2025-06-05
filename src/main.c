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
#include "render.h"

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
 * @brief Setup a demonstration scene
 */
static void setup_demo_scene(void) {
    // Clear any existing scene
    scene_clear();
    
    // Create spheres
    Sphere sphere1 = sphere_create(
        vec3_create(0.0f, 0.0f, -1.0f),    // center
        0.5f,                               // radius
        color_create(0.8f, 0.3f, 0.3f)     // red material
    );
    
    Sphere sphere2 = sphere_create(
        vec3_create(-1.0f, 0.0f, -1.0f),   // center
        0.5f,                               // radius
        color_create(0.3f, 0.3f, 0.8f)     // blue material
    );
    
    Sphere sphere3 = sphere_create(
        vec3_create(1.0f, 0.0f, -1.0f),    // center
        0.5f,                               // radius
        color_create(0.3f, 0.8f, 0.3f)     // green material
    );
    
    // Create ground plane
    Plane ground = plane_create_xz(
        -0.5f,                              // y position
        color_create(0.5f, 0.5f, 0.5f)     // gray material
    );
    
    // Add objects to scene
    scene_add_object(sphere_to_hittable(&sphere1));
    scene_add_object(sphere_to_hittable(&sphere2));
    scene_add_object(sphere_to_hittable(&sphere3));
    scene_add_object(plane_to_hittable(&ground));
    
    // Set up lighting
    Vec3 light_pos = vec3_create(-2.0f, 4.0f, 1.0f);
    Color light_color = color_create(1.0f, 1.0f, 1.0f);  // White light
    scene_set_light(light_pos, light_color);
    
    Color ambient_color = color_create(0.1f, 0.1f, 0.1f);  // Dim ambient
    scene_set_ambient(ambient_color);
    
    printf("Demo scene created:\n");
    printf("  - 3 spheres (red, blue, green)\n");
    printf("  - 1 ground plane (gray)\n");
    printf("  - Point light with ambient lighting\n");
}

/**
 * @brief Render the ray-traced scene
 */
static void render_ray_traced_scene(FILE *output, int width, int height) {
    // Setup the scene
    setup_demo_scene();
    
    // Create camera
    Vec3 camera_pos = vec3_create(0.0f, 0.0f, 1.0f);
    Vec3 target = vec3_create(0.0f, 0.0f, -1.0f);
    Vec3 up = vec3_create(0.0f, 1.0f, 0.0f);
    
    float aspect_ratio = (float)width / (float)height;
    Camera camera = camera_create_perspective(
        camera_pos, target, up, 
        45.0f,        // 45 degree field of view
        aspect_ratio, 
        width, height
    );
    
    printf("Camera setup: perspective projection, 45° FOV\n");
    
    // Render the scene
    render_scene(&camera, output);
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
    printf("Rendering %dx%d ray-traced image to '%s'\n", image_width, image_height, output_filename);
    
    // Open output file
    FILE *output = fopen(output_filename, "w");
    if (!output) {
        fprintf(stderr, "Error: Could not open output file '%s'\n", output_filename);
        return 1;
    }
    
    // Render the ray-traced scene
    render_ray_traced_scene(output, image_width, image_height);
    
    fclose(output);
    
    printf("Ray tracing complete! Output written to '%s'\n", output_filename);
    printf("\nScene contents:\n");
    printf("  - 3 colored spheres with Lambertian shading\n");
    printf("  - Ground plane for depth reference\n");
    printf("  - Point light source with ambient lighting\n");
    printf("  - Perspective camera with 45° field of view\n");
    printf("\nTo view the image:\n");
    printf("  - On macOS: open %s\n", output_filename);
    printf("  - On Linux: display %s  (ImageMagick)\n", output_filename);
    printf("  - On Windows: Use any image viewer that supports PPM\n");
    
    return 0;
} 