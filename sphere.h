#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
using namespace std;

class sphere : public hittable{
    
    private:
        point3 center;
        double radius;

    public:
        sphere(const point3& center, double radius) : center(center), radius(fmax(0,radius)){}
        
        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec ){
            auto oc = center - r.origin();
            auto a = dot(r.direction(), r.direction());
            auto h = dot(r.direction(), oc);
            auto c = dot(oc,oc) - radius * radius;

            auto discriminant = h*h - a*c;
            if(discriminant < 0) return false;
            
            auto sqtrd = sqrt(discriminant);

            // find root

            auto root = (h - sqtrd)/a;
            if(root <= ray_tmin || ray_tmax <= root){
                root = (h + sqtrd)/a;
                if(root <= ray_tmin || ray_tmax <= root)
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center)/radius;
            rec.set_face_normal(r, outward_normal);

            return true;
        }
};

#endif