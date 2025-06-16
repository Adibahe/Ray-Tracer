#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"


using color = vec3;

double liner_space_to_gamma(double a){
    if(a > 0.0) return sqrt(a);
    return 0;
}

void write_color(ostream& out, const color& pixel_color){
    auto r = liner_space_to_gamma(pixel_color.x());
    auto g = liner_space_to_gamma(pixel_color.y());
    auto b = liner_space_to_gamma(pixel_color.z());
        


    static const interval intensity(0.000, 0.999);
    int rbyte = int(255.999 * intensity.clamp(r));
    int gbyte = int(255.999 * intensity.clamp(g));
    int bbyte = int(255.999 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte <<' ';
}

#endif