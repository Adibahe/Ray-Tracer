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

class dielectric : public material{
    public: 

        dielectric(double refractive_index): refractive_index(refractive_index){}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)const override{
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? (1.0/ refractive_index) : refractive_index;

            vec3 unit_dir = unit_vector(r_in.direction());
            double cos_theta = std::fmin(dot(-unit_dir, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

            vec3 direction;
            if(ri * sin_theta > 1.0 || reflectance(cos_theta, ri) > random_double()){
                direction = reflection(unit_dir, rec.normal);
            }
            else{
                direction = refract(unit_dir, rec.normal, ri, cos_theta);
            }

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        double refractive_index;

        static double reflectance(double cosine, double refraction_index) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - refraction_index) / (1 + refraction_index);
            r0 = r0*r0;
            return r0 + (1-r0)*std::pow((1 - cosine),5);
        }
};

#endif