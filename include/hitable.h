#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"

struct hit_record {
	float t;
	vec3 p;
	vec3 normal;
};

class hitable {
public: virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif // HITABLE_H