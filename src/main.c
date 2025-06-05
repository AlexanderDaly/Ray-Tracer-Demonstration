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
 * @brief Create a camera for the demo scene
 */
static Camera create_demo_camera(int width, int height) {
    Vec3 camera_pos = vec3_create(0.0f, 0.0f, 0.0f);
    Vec3 target = vec3_create(0.0f, 0.0f, -1.0f);
    Vec3 up = vec3_create(0.0f, 1.0f, 0.0f);
    
    float aspect_ratio = (float)width / (float)height;
    float viewport_height = 2.0f;
    float viewport_width = viewport_height * aspect_ratio;
    
    return camera_create_orthographic(camera_pos, target, up,
                                     viewport_width, viewport_height,
                                     width, height);
}

/**
 * @brief Create the demo scene
 */
static Scene create_demo_scene(void) {
    // Create scene with a nice blue sky background
    Color sky_color = color_create(0.5f, 0.7f, 1.0f);
    Scene scene = scene_create(sky_color);
    
    // Create a red sphere in the center
    static Sphere sphere = {0};
    sphere = sphere_create(
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
    printf("Rendering %dx%d ray-traced scene to '%s'\n", image_width, image_height, output_filename);
    printf("Scene: Red sphere and blue plane with point lighting\n");
    
    // Open output file
    FILE *output = fopen(output_filename, "w");
    if (!output) {
        fprintf(stderr, "Error: Could not open output file '%s'\n", output_filename);
        return 1;
    }
    
    // Create camera and scene
    Camera camera = create_demo_camera(image_width, image_height);
    Scene scene = create_demo_scene();
    
    // Render the scene
    render_scene(&camera, &scene, output);
    
    // Cleanup
    fclose(output);
    
    printf("Render complete! Output written to '%s'\n", output_filename);
    printf("\nTo view the image:\n");
    printf("  - On macOS: open %s\n", output_filename);
    printf("  - On Linux: display %s  (ImageMagick)\n", output_filename);
    printf("  - On Windows: Use any image viewer that supports PPM\n");
    
    return 0;
} 