#ifndef SPHERE_H
#define SPHERE_H

#include <stdlib.h>
#include "hitable.h"

//double drand48(void);

class sphere : public hitable {
public: 
	sphere() {}
	sphere(vec3 cen, double r, material* ptr) : center(cen), radius(r), mat_ptr(ptr) {};
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
	
	// Members
	vec3 center;
	double radius;
	material* mat_ptr;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = 2.0 * dot(oc, r.direction());
	double c = dot(oc, oc) - radius * radius;
	double discriminant = b * b - 4 * a * c;

	if (discriminant > 0) {
		double tmp = (-b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
		if (tmp < t_max && tmp > t_min) {
			rec.t = tmp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = unit_vector((rec.p - center) / radius);
			rec.mat_ptr = mat_ptr;
			return true;
		}

		tmp = (-b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
		if (tmp < t_max && tmp > t_min) {
			rec.t = tmp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}



// Uncomment when debugging with VS Code - Windows doesn't have drand48
//double drand48(void) {
//	return (double)rand() / (RAND_MAX + 1.0);
//}



#endif // SPHERE_H