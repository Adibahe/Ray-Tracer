#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
  public:

    double aspect_ratio = 1.0; 
    int image_width  = 100;
    int samples_per_pixel = 10;
    point3 center;
    int max_depth = 10;

    void render(const hittable& world) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for(int sample = 0; sample < samples_per_pixel; sample++){
                    pixel_color += color_ray(get_ray(i,j), max_depth, world);
                }
                write_color(cout, pixel_samples_scale * pixel_color);
            }
            cout << '\n';
        }

        std::clog << "\rDone.                 \n";
    }

  private:
    int    image_height;        
    point3 pixel00_loc;    
    vec3   pixel_delta_u;  
    vec3   pixel_delta_v;
    double pixel_samples_scale;

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const{
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        
        return ray (center, pixel_sample - center);
    }

    vec3 sample_square() const{
        return vec3(random_double() - 0.5 , random_double() - 0.5, 0);
    }

    color color_ray(const ray& r, int depth, const hittable& world) const{
        
        if(depth <= 0 ) return color(0,0,0);

        hit_record rec;

        if(world.hit(r, interval(0.001, infinity), rec)){
            ray scattered;
            color attenuation;

            if(rec.mat -> scatter(r, rec, attenuation, scattered)){
                return attenuation * color_ray(scattered, depth - 1, world);
            }
                return color(0,0,0);
        }

        vec3 unit_dir = unit_vector(r.direction());
        auto a = 0.5 * (unit_dir.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
};

#endif