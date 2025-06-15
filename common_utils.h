#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include<memory>
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

// common headers

#include "color.h"
#include "ray.h"
#include "interval.h"
#include "vec3.h"

#endif