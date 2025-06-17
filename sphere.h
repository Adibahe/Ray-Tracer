#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
using namespace std;

class sphere : public hittable{
    
    private:
        point3 center;
        double radius;
        shared_ptr<material> mat;

    public:
        sphere(const point3& center, double radius, shared_ptr<material> mat) : center(center), radius(fmax(0,radius)), mat(mat){}
        
        bool hit(const ray& r, interval ray_t, hit_record& rec ) const override{
            auto oc = center - r.origin();
            auto a = dot(r.direction(), r.direction());
            auto h = dot(r.direction(), oc);
            auto c = dot(oc,oc) - radius * radius;

            auto discriminant = h*h - a*c;
            if(discriminant < 0) return false;
            
            auto sqtrd = sqrt(discriminant);

            // find root

            auto root = (h - sqtrd)/a;
            if(!ray_t.surrounds(root)){
                root = (h + sqtrd)/a;
                if(!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center)/radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }
};

#endif