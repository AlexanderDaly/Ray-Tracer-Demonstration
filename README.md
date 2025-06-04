# Ray Tracer Demonstration (C Edition)

A minimal yet extensible CPU-based ray tracer written in ISO C11.

## Project Overview

**Project Sponsor:** Hondo (Alexander Daly)  
**Project Manager & Architect:** Morpheus  
**Target Demo Date:** July 15, 2025 (6-week schedule)  
**Current Version:** v0.1-dev (Sprint 0: Project Setup)

## Goals

Build a CLI program that outputs a PPM image of a simple scene containing:
- At least one sphere and one plane
- Point light with Lambertian shading
- Configurable image dimensions

## Quick Start

### Prerequisites

- GCC 13+ or Clang 17+ with C11 support
- Make
- gcovr (for coverage reports, optional)
- clang-format (for code formatting, optional)

### Build & Run

```bash
# Clone and setup
git clone <repository>
cd ray-tracer-demonstration

# Install test dependencies
make deps

# Build and run
make all
./bin/raydemo -w 400 -h 225 -o output/demo.ppm

# View the result
open output/demo.ppm  # macOS
# or
display output/demo.ppm  # Linux with ImageMagick
```

### Development Workflow

```bash
# Debug build with sanitizers
make debug

# Run tests
make test

# Generate coverage report
make coverage

# Format code
make format

# Clean build artifacts
make clean
```

## Project Structure

```
raytracer_demo_c/
├── include/           # Header files
│   ├── vec3.h        # 3D vector mathematics
│   ├── color.h       # RGB color operations
│   ├── ray.h         # Ray representation
│   ├── camera.h      # Camera and ray generation
│   ├── hit.h         # Hit detection interface
│   ├── sphere.h      # Sphere geometry
│   └── plane.h       # Plane geometry
├── src/              # Implementation files
│   ├── main.c        # CLI entry point
│   ├── vec3.c ray.c color.c camera.c
│   ├── sphere.c plane.c
│   └── render.c      # Rendering loop (future)
├── tests/            # Unit tests (Unity framework)
├── output/           # Generated images
├── examples/         # Example scenes
├── docs/             # Documentation
├── Makefile          # Build configuration
└── README.md
```

## Architecture

### Core Data Structures

| Type        | Description                              | Key Functions                  |
|-------------|------------------------------------------|--------------------------------|
| `Vec3`      | 3D vector with x, y, z components       | `vec3_add`, `vec3_dot`, `vec3_normalize` |
| `Color`     | RGB color (same layout as Vec3)         | `color_scale`, `color_to_u8`   |
| `Ray`       | Ray with origin and direction            | `ray_at`, `ray_create`         |
| `Camera`    | Camera with viewport parameters          | `camera_get_ray`               |
| `HitRecord` | Intersection information                 | Contains point, normal, t      |
| `Sphere`    | Sphere with center and radius            | `sphere_hit`                   |
| `Plane`     | Infinite plane with point and normal     | `plane_hit`                    |

### Extensible Hit Interface

Objects implement hit detection via function pointers:

```c
typedef bool (*HitFunction)(const Hittable *object, const Ray *ray, 
                           float t_min, float t_max, HitRecord *hit_rec);
```

This allows easy addition of new primitive types without modifying existing code.

## Command Line Usage

```bash
./raydemo [OPTIONS]

Options:
  -w, --width WIDTH    Image width in pixels (default: 400)
  -h, --height HEIGHT  Image height in pixels (default: 225)  
  -o, --output FILE    Output PPM file (default: output.ppm)
  --help               Show help message

Examples:
  ./raydemo -w 800 -h 600 -o high_res.ppm
  ./raydemo --width 1920 --height 1080 --output hd_render.ppm
```

## Development Status

### ✅ Sprint 0: Project Setup (Current)
- [x] Repository structure
- [x] Makefile with C11 flags
- [x] Header files with API definitions
- [x] Basic CLI argument parsing
- [x] PPM output support
- [ ] CI/CD setup (GitHub Actions)
- [ ] Unity test framework integration

### 🔄 Sprint 1: Math Kernel (Next)
- [ ] Complete `vec3.c` implementation
- [ ] Unit tests for vector operations
- [ ] Color conversion functions
- [ ] Ray intersection math

### 📋 Future Sprints
- Sprint 2: Core Types (Ray, Camera, HitRecord)
- Sprint 3: Geometry & Intersection (Sphere, Plane)
- Sprint 4: Renderer & Shading
- Sprint 5: Polish & Demo

## Quality Metrics

- **Unit test coverage:** Target ≥80%
- **Performance:** 400×225 render in ≤5s on 4-core Intel CPU
- **Memory safety:** Clean Valgrind run
- **Code style:** Consistent formatting via clang-format

## Contributing

1. Follow the C11 standard strictly
2. Write unit tests for new functions
3. Run `make format` before committing
4. Ensure `make test` passes
5. Use descriptive commit messages

## Roadmap (Post-v0.1)

- **v0.2:** Anti-aliasing & stochastic sampling
- **v0.3:** Reflective materials & recursive rays  
- **v0.4:** Bounding Volume Hierarchy (BVH)
- **v0.5:** Multithreading support
- **v1.0:** WebAssembly port

## License

[License TBD - to be discussed with project sponsor]

---

**Project Management:** This project follows agile methodology with 1-week sprints.  
**Communication:** GitHub Issues for task tracking, weekly video demos.  
**Last Updated:** June 2025 by Morpheus
