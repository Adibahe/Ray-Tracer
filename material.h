#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material{
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered) const{
            return false; 
        }
};

class lambertian : public material{
    public:
        lambertian(const color& albedo) : albedo(albedo){}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            double p = 0.7;
            double s = 1e-8;

            auto scatter_direction = rec.normal + random_unit_vector();

            if(fabs(scatter_direction.e[0]) < s && fabs(scatter_direction.e[1]) < s && fabs(scatter_direction.e[2]) < s){
                scatter_direction = rec.normal;
            }
            scattered = ray(rec.p, scatter_direction);
            if(random_double() < p){
                attenuation = albedo / p;
            }
                attenuation = albedo;
                return true;
        }

    private:
        color albedo;
};

class metal : public material{
    public:
        double p = 0.3;
        metal(const color& albedo, double fuzz_f): albedo(albedo), fuzz_factor(fuzz_f < 1 ? fuzz_f: 1){}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            vec3 reflected = reflection(r_in.direction(),rec.normal);
            reflected = unit_vector(reflected) + (fuzz_factor * random_unit_vector());
            scattered = ray(rec.p, reflected);

            if(random_double() < p){
                attenuation = albedo / p;
            }
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        double fuzz_factor;
};

#endif