#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <chrono>

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

    long long total_rays = static_cast<long long>(image_width) * image_height * samples_per_pixel;
    long long rays_traced = 0;

    auto start_time = std::chrono::steady_clock::now();

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                pixel_color += color_ray(get_ray(i, j), max_depth, world);
                rays_traced++; // Count one ray per sample
            }
            write_color(std::cout, pixel_samples_scale * pixel_color);
        }

        // ETA calculation and display after each scanline
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - start_time;

        double progress = static_cast<double>(rays_traced) / total_rays;
        double total_estimated_time = elapsed.count() / (progress > 0 ? progress : 1e-6);
        double time_remaining = total_estimated_time - elapsed.count();

        int eta_minutes = static_cast<int>(time_remaining / 60);
        int eta_seconds = static_cast<int>(time_remaining) % 60;

        std::clog << "\rScanlines remaining: " << (image_height - j - 1)
                  << " | ETA: " << eta_minutes << "m " << eta_seconds << "s"
                  << "   " << std::flush;
    }

    auto end_time = std::chrono::steady_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();

    int total_minutes = static_cast<int>(total_duration / 60);
    int total_seconds = static_cast<int>(total_duration % 60);

    std::clog << "\rDone.                                                 \n"
    << "Total render time: " << total_minutes << "m " << total_seconds << "s\n";

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

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;
        center = lookfrom;

        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        vec3 viewport_u = viewport_width * u;
        vec3 viewport_v = viewport_height * -v;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        auto disk_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        disk_u = u * disk_radius;
        disk_v = v * disk_radius;
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        auto origin = (defocus_angle <= 0) ? center : defocus_sample();
        return ray(origin, pixel_sample - origin);
    }

    vec3 sample_square() const {
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * disk_u) + (p[1] * disk_v);
    }

    color color_ray(const ray& r, int depth, const hittable& world) const {
        if (depth <= 0) return color(0, 0, 0);

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered)) {
                return attenuation * color_ray(scattered, depth - 1, world);
            }
            return color(0, 0, 0);
        }

        vec3 unit_dir = unit_vector(r.direction());
        auto a = 0.5 * (unit_dir.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif
