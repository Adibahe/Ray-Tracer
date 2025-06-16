#include "common_utils.h"
#include<iostream>
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include <memory>

using namespace std;

// set color


int main() {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,0), 100));

    camera cam;
    
    cam.aspect_ratio = 16.0/9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.center = point3(0,0,0);
    cam.max_depth = 50;
    cam.render(world);
    
}