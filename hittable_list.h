#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>


class hittable_list : public hittable{
    public:
        vector<shared_ptr<hittable>> scene_objects;

        hittable_list(){}
        hittable_list(shared_ptr<hittable>& object){ add(object); }
        
        void add(const shared_ptr<hittable>& object){ scene_objects.push_back(object);}

        void clear(){scene_objects.clear();}
        
        

        bool hit(const ray& r, interval ray_t, hit_record& rec )const override{
            hit_record temp_rec;
            bool hit_anything = false;
            auto closet_t = ray_t.max;

            for(auto& object : scene_objects){
                if(object -> hit(r, interval(ray_t.min, closet_t), temp_rec)){
                    hit_anything = true;
                    closet_t = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }
};

#endif