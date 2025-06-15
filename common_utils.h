#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include<memory>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<limits>


using std::make_shared;
using std::shared_ptr;

using namespace std;



// constants
const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees){
    return degrees * pi / 180.0;
}

inline double random_double(){
    return rand()/(RAND_MAX + 1.0);
}

inline double random_double(double min, double max){
    return min + (max - min)*random_double();
}

// common headers

#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"

#endif