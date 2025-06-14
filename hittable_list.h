#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using namespace std;

class hittable_list : public hittable{
    public:
        vector<shared_ptr<hittable>> scene_objects;

        hittable_list(){}
        
        void add(shared_ptr<hittable>& object){ scene_objects.push_back(object);}

        void clear(){scene_objects.clear();}
        
        hittable_list(shared_ptr<hittable>& object){ add(object); }

        bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec ){
            hit_record temp_rec;
            bool hit_anything = false;
            auto closet_t = ray_tmax;

            for(auto& object : scene_objects){
                if(object -> hit(r, ray_tmin, closet_t, temp_rec)){
                    hit_anything = true;
                    closet_t = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }
};

#endif