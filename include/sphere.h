#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "onb.h"

//double drand48(void);

class sphere : public hittable {
public:
    // Stationary Sphere
    sphere(const point3& static_center, double radius, shared_ptr<material> mat) 
        : center(static_center, vec3(0,0,0)), radius(std::fmax(0, radius)), mat(mat) 
    {
        vec3 rvec = vec3(radius, radius, radius);
        bbox = aabb(static_center - rvec, static_center + rvec);
    }

    // Moving Sphere
    sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat) 
        : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat)
    {
        vec3 rvec = vec3(radius, radius, radius);
        aabb box1(center.at(0) - rvec, center.at(0) + rvec);
        aabb box2(center.at(1) - rvec, center.at(1) + rvec);
        bbox = aabb(box1, box2);
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        point3 current_center = center.at(r.time());
        vec3 oc = current_center - r.origin();
        double a = r.direction().length_squared();
        double h = dot(r.direction(), oc);
        double c = oc.length_squared() - radius * radius;

        double discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        double sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        double root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - current_center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

    double pdf_value(const point3& origin, const vec3& direction) const override {
        hit_record rec;
        if (!this->hit(ray(origin, direction), interval(0.001, infinity), rec))
            return 0;

        double dist_squared = (center.at(0) - origin).length_squared();
        double cos_theta_max = std::sqrt(1 - radius * radius / dist_squared);
        double solid_angle = 2.0 * pi * (1 - cos_theta_max);

        return 1.0 / solid_angle;
    }

    vec3 random(const point3& origin) const override {
        vec3 direction = center.at(0) - origin;
        double distance_squared = direction.length_squared();
        onb uvw(direction);
        return uvw.transform(random_to_sphere(radius, distance_squared));
    }

private:
    ray center;
    double radius;
    shared_ptr<material> mat;
    aabb bbox;

    /*
    Finds the uv coordinates on a sphere of radius 1, centered at origin.
    @param p, the point in model space to find the uv
    @param u, returned value from [0, 1] of angle around Y axis from X = -1
    @param v, returned value from [0, 1] of angle from Y = -1 to Y = +1
    */
    static void get_sphere_uv(const point3& p, double& u, double& v) {
        double theta = std::acos(-p.y());
        double phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }

    static vec3 random_to_sphere(double radius, double distance_squared) {
        double r1 = random_double();
        double r2 = random_double();
        double z = 1 + r2 * (std::sqrt(1 - radius * radius / distance_squared) - 1);

        double phi = 2.0 * pi * r1;
        double x = std::cos(phi) * std::sqrt(1.0 - z * z);
        double y = std::sin(phi) * std::sqrt(1.0 - z * z);

        return vec3(x, y, z);
    }
};

#endif // SPHERE_H