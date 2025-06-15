#include "common_utils.h"
#include<iostream>
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <memory>

using namespace std;

// set color
color color_ray(const ray& r, const hittable& world){
    hit_record rec;
    if(world.hit(r, interval(0, infinity), rec)){return 0.5 * (rec.normal + color(1,1,1));}

    vec3 unit_dir = unit_vector(r.direction());
    auto a = 0.5 * (unit_dir.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {

    auto aspect_ratio = 16.0/9.0;
    int image_width = 400;

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1)? 1 : image_height;
    
    //world
    hittable_list world;

    world.add(make_shared <sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared <sphere>(point3(0,-100.5,-1), 100));

    // camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(100, 0, 0);

    // viewport

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u/image_width;
    auto pixel_delta_v = viewport_v/image_height;

    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

   for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_dir = pixel_center - camera_center;

            ray r(camera_center, ray_dir);

            color pixel_color = color_ray(r, world);
            write_color(cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}