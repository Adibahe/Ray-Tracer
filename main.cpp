#include "common_utils.h"
#include<iostream>
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include <memory>

using namespace std;

// set color


int main() {
    hittable_list world;

    // === Function to add a ringed planet ===
    auto add_ringed_planet = [&](point3 center, double radius,
                                 shared_ptr<material> planet_mat,
                                 shared_ptr<material> ring_mat,
                                 double inner_ring, double outer_ring, int count) {
        world.add(make_shared<sphere>(center, radius, planet_mat));

        for (int i = 0; i < count; ++i) {
            double angle = random_double(0, 2 * pi);
            double dist = random_double(inner_ring, outer_ring);
            double height = random_double(-0.05, 0.05);  // thin ring
            point3 pos = center + vec3(cos(angle) * dist, height, sin(angle) * dist);
            world.add(make_shared<sphere>(pos, 0.05, ring_mat));
        }
    };

    // === Ringed Planet 1: Blue metal + Cyan ring ===
    auto blue_metal = make_shared<metal>(color(0.3, 0.4, 1.0), 0.0);
    auto cyan_ring = make_shared<lambertian>(color(0.2, 1.0, 1.0));
    add_ringed_planet(point3(0, 0, 0), 1.0, blue_metal, cyan_ring, 1.3, 2.2, 200);

    // === Ringed Planet 2: Pink metal + Lime ring ===
    auto pink_metal = make_shared<metal>(color(1.0, 0.4, 0.7), 0.0);
    auto lime_ring = make_shared<lambertian>(color(0.6, 1.0, 0.2));
    add_ringed_planet(point3(6, 0, -2), 0.8, pink_metal, lime_ring, 1.1, 1.9, 150);

    // === Ringed Planet 3: Orange diffuse + Yellow ring ===
    auto orange_diffuse = make_shared<lambertian>(color(1.0, 0.5, 0.2));
    auto yellow_ring = make_shared<lambertian>(color(1.0, 1.0, 0.2));
    add_ringed_planet(point3(-6, 0.1, -3), 0.9, orange_diffuse, yellow_ring, 1.1, 2.0, 180);

    // === Ringed Planet 4: Glass + purple ring ===
    auto glass = make_shared<dielectric>(1.5);
    auto purple_ring = make_shared<lambertian>(color(0.8, 0.3, 1.0));
    add_ringed_planet(point3(0, -1, 6), 1.0, glass, purple_ring, 1.2, 2.1, 180);

    // === Optional: Starfield or tiny moons ===
    for (int i = 0; i < 200; ++i) {
        point3 pos(random_double(-50, 50), random_double(-10, 10), random_double(-40, 10));
        auto star_color = color::random(0.8, 1.0);
        auto star = make_shared<lambertian>(star_color);
        world.add(make_shared<sphere>(pos, random_double(0.02, 0.05), star));
    }

    // === Camera ===
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 40;
    cam.lookfrom = point3(10, 5, 15);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.render(world);
}
