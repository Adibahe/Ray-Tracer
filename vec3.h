#ifndef VEC3_H
#define VEC3_H


using namespace std;

class vec3{
    public:
        double e[3];
        vec3() : e{0,0,0} {};
        vec3 (double e1, double e2, double e3) : e{e1, e2, e3} {};

        double x() const {return e[0];}
        double y() const {return e[1];}
        double z() const {return e[2];}

        vec3 operator -() const{return vec3(-e[0], -e[1], -e[2]);}
        double operator [](int i) const{return e[i];}
        double& operator [](int i) {return e[i];}

        vec3& operator +=(const vec3& v) {
            e[0] += v[0];
            e[1] += v[1];
            e[2] += v[2];
            
            return *this;
        }

        vec3& operator*=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(double t) {
            return *this *= 1/t;
        }

        double length() const {
            return std::sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        static vec3 random(){
            return vec3(random_double(),random_double(),random_double());
        }

        static vec3 random(double min, double max){
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

};

using point3 = vec3;

inline ostream& operator<< (ostream& out, const vec3& v){
    return out << v[0] << ' ' << v[1] << ' ' << v[2];
}

inline vec3 operator +(const vec3& u , const vec3& v){
    return vec3(u[0] + v[0] , u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator -(const vec3& u, const vec3& v){
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}
   
inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 random_unit_vector(){
    while(true){
        auto p = vec3::random(-1,1);
        auto len = p.length_squared();

        if( 1e-160 < len <= 1) return p/sqrt(len);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal){
    vec3 rand_vec = random_unit_vector();
    if(dot(rand_vec, normal) > 0.0)
        return rand_vec;
    else return -rand_vec;
}

inline vec3 reflection(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& ray, const vec3& n, double etai_by_etat, double cos_theta){
    vec3 ray_out_prep = etai_by_etat*(ray + cos_theta * n);
    vec3 ray_out_parallel = -sqrt(fabs(1.0 - ray_out_prep.length_squared()))*n;
    return ray_out_parallel + ray_out_prep;
}
#endif