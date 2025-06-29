#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
public:
    double aspect_ratio = 1.0; 
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;
    double vfov = 90;
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0);

    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    pixel_color += color_ray(get_ray(i, j), max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
            std::cout << '\n';
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int image_height;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    double pixel_samples_scale;
    point3 center;
    vec3 u, v, w;
    vec3 disk_u, disk_v;

    // Initialize camera parameters
    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;
        center = lookfrom;

        // Calculate the vertical field of view in radians
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate camera coordinate system (right, up, forward)
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate viewport's horizontal and vertical dimensions
        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        // Set up the pixel deltas
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Upper left corner of the viewport
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate disk radius for depth of field (defocus)
        auto disk_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        disk_u = u * disk_radius;
        disk_v = v * disk_radius;
    }

    // Generate a ray for the pixel at (i, j)
    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        auto origin = (defocus_angle <= 0) ? center : defocus_sample();
        return ray(origin, pixel_sample - origin);
    }

    // Generate a random offset for antialiasing
    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    // Generate a random sample point on the defocus disk
    point3 defocus_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * disk_u) + (p[1] * disk_v);
    }

    // Trace a ray and calculate its color
    color color_ray(const ray& r, int depth, const hittable& world) const {
        if (depth <= 0) return color(0, 0, 0);

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * color_ray(scattered, depth - 1, world);
            }
            return color(0, 0, 0); // No scattering, return black
        }

        // Sky background color
        vec3 unit_dir = unit_vector(r.direction());
        auto a = 0.5 * (unit_dir.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif
