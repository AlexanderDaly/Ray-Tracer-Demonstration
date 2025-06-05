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
#include "scene.h"
#include "render.h"

/**
 * @brief Print usage information
 */
static void print_usage(const char *program_name) {
    printf("Ray Tracer Demonstration v0.1 - Now with ACTUAL RAY TRACING!\n");
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("\nOptions:\n");
    printf("  -w, --width WIDTH    Image width in pixels (default: 400)\n");
    printf("  -h, --height HEIGHT  Image height in pixels (default: 225)\n");
    printf("  -o, --output FILE    Output PPM file (default: output.ppm)\n");
    printf("  -s, --samples N      Samples per pixel for anti-aliasing (default: 1)\n");
    printf("  -d, --depth N        Maximum ray bounce depth (default: 10)\n");
    printf("  --help               Show this help message\n");
    printf("\nExample:\n");
    printf("  %s -w 800 -h 600 -s 4 -o render.ppm\n", program_name);
    printf("\nThis will render a scene containing:\n");
    printf("  - Red sphere in the center\n");
    printf("  - Green ground plane\n");
    printf("  - Point light with shadows\n");
    printf("  - Lambertian shading\n");
}

/**
 * @brief Create the demo scene
 */
static Scene create_demo_scene(void) {
    // Create scene with a nice blue sky background
    Color sky_color = color_create(0.5f, 0.7f, 1.0f);
    Scene scene = scene_create(sky_color);
    
    // Create a red sphere in the center
    Sphere sphere = sphere_create(
        vec3_create(0.0f, 0.0f, -1.0f),  // center
        0.5f,                            // radius
        color_create(0.8f, 0.3f, 0.3f)   // red color
    );
    scene_add_object(&scene, sphere_to_hittable(&sphere));
    
    // Create a green ground plane
    Plane ground = plane_create_xz(
        -0.5f,                           // y position
        color_create(0.3f, 0.8f, 0.3f)   // green color
    );
    scene_add_object(&scene, plane_to_hittable(&ground));
    
    // Add a second smaller sphere for interest
    Sphere small_sphere = sphere_create(
        vec3_create(-1.0f, 0.0f, -1.0f), // center (to the left)
        0.3f,                           // smaller radius
        color_create(0.3f, 0.3f, 0.8f)  // blue color
    );
    scene_add_object(&scene, sphere_to_hittable(&small_sphere));
    
    // Add a point light above and to the side
    PointLight light;
    light.position = vec3_create(1.0f, 1.0f, 0.0f);
    light.color = color_white();
    light.intensity = 1.5f;
    scene_add_light(&scene, light);
    
    // Add a second light for softer shadows
    PointLight light2;
    light2.position = vec3_create(-0.5f, 1.5f, 0.5f);
    light2.color = color_create(1.0f, 0.9f, 0.8f); // Slightly warm
    light2.intensity = 0.8f;
    scene_add_light(&scene, light2);
    
    return scene;
}

/**
 * @brief Main entry point
 */
int main(int argc, char *argv[]) {
    // Default parameters
    int image_width = 400;
    int image_height = 225;
    const char *output_filename = "output.ppm";
    int samples_per_pixel = 1;
    int max_depth = 10;
    
    // Command line option structure
    static struct option long_options[] = {
        {"width",   required_argument, 0, 'w'},
        {"height",  required_argument, 0, 'h'},
        {"output",  required_argument, 0, 'o'},
        {"samples", required_argument, 0, 's'},
        {"depth",   required_argument, 0, 'd'},
        {"help",    no_argument,       0, 0},
        {0, 0, 0, 0}
    };
    
    // Parse command line arguments
    int option_index = 0;
    int c;
    
    while ((c = getopt_long(argc, argv, "w:h:o:s:d:", long_options, &option_index)) != -1) {
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
                
            case 's':
                samples_per_pixel = atoi(optarg);
                if (samples_per_pixel <= 0) {
                    fprintf(stderr, "Error: Samples must be positive\n");
                    return 1;
                }
                break;
                
            case 'd':
                max_depth = atoi(optarg);
                if (max_depth <= 0) {
                    fprintf(stderr, "Error: Depth must be positive\n");
                    return 1;
                }
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
    printf("Samples per pixel: %d\n", samples_per_pixel);
    printf("Max ray depth: %d\n", max_depth);
    printf("\n");
    
    // Create the scene
    Scene scene = create_demo_scene();
    printf("Scene created with %d objects and %d lights\n", 
           scene.object_count, scene.light_count);
    
    // Set up camera (perspective view)
    float aspect_ratio = (float)image_width / (float)image_height;
    Camera camera = camera_create_perspective(
        vec3_create(0.0f, 0.0f, 0.0f),   // camera position
        vec3_create(0.0f, 0.0f, -1.0f),  // look at point
        vec3_create(0.0f, 1.0f, 0.0f),   // up vector
        45.0f,                          // field of view
        aspect_ratio,                   // aspect ratio
        image_width,                    // image width
        image_height                    // image height
    );
    
    // Set up render settings
    RenderSettings settings = render_settings_default();
    settings.samples_per_pixel = samples_per_pixel;
    settings.max_depth = max_depth;
    settings.show_progress = true;
    
    // Open output file
    FILE *output = fopen(output_filename, "w");
    if (!output) {
        fprintf(stderr, "Error: Could not open output file '%s'\n", output_filename);
        return 1;
    }
    
    // Render the scene!
    printf("Starting ray tracing render...\n");
    bool success = render_scene_to_file(&scene, &camera, &settings, output);
    fclose(output);
    
    if (success) {
        printf("Render complete! Output written to '%s'\n", output_filename);
        printf("\nTo view the image:\n");
        printf("  - On macOS: open %s\n", output_filename);
        printf("  - On Linux: display %s  (ImageMagick)\n", output_filename);
        printf("  - On Windows: Use any image viewer that supports PPM\n");
        printf("\nYou should see:\n");
        printf("  🔴 Red sphere in the center with realistic shading\n");
        printf("  🔵 Blue sphere to the left\n");
        printf("  🟢 Green ground plane below\n");
        printf("  ☀️  Shadows and lighting effects\n");
        printf("  🌌 Blue sky background\n");
    } else {
        fprintf(stderr, "Error: Render failed\n");
        return 1;
    }
    
    return 0;
} 