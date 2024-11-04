#ifndef HITTABLE_H
#define HITTABLE_H

#include "aabb.h"

class material;

class hit_record {
public:
	point3 p;
	vec3 normal;
	shared_ptr<material> mat;
	double t;
	bool front_face;

	/*Sets the hit record normal vector*/
	void set_face_normal(const ray& r, vec3& outward_normal) {
		make_unit_vector(outward_normal);
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public: 
	virtual ~hittable() = default;
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
	virtual aabb bounding_box() const = 0;
};

#endif // HITTABLE_H