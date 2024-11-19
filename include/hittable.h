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
	double u;
	double v;
	bool front_face;

	/*Sets the hit record normal vector*/
	void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public: 
	virtual ~hittable() = default;
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
	virtual aabb bounding_box() const = 0;
	virtual double pdf_value(const point3& origin, const vec3& direction) const { return 0.0; }
	virtual vec3 random(const point3& origin) const { return vec3(1, 0, 0); }
};

class translate : public hittable {
public:
	translate(shared_ptr<hittable> object, const vec3& offset) : object(object), offset(offset) { bbox = object->bounding_box() + offset; }
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		ray offset_r(r.origin() - offset, r.direction(), r.time());

		if (!object->hit(offset_r, ray_t, rec))
			return false;
		rec.p += offset;

		return true;
	}
	aabb bounding_box() const override { return bbox; }
private:
	shared_ptr<hittable> object;
	vec3 offset;
	aabb bbox;
};

class rotate_y : public hittable {
public:
	//TODO: Add rotations for other axes
	rotate_y(shared_ptr<hittable> object, double deg_x, double deg_y, double deg_z) : object(object) {
		double rad_x = degrees_to_radians(deg_x);
		double rad_y = degrees_to_radians(deg_y);
		double rad_z = degrees_to_radians(deg_z);

		sin_x = std::sin(rad_x);
		sin_y = std::sin(rad_y);
		sin_z = std::sin(rad_z);

		cos_x = std::cos(rad_x);
		cos_y = std::cos(rad_y);
		cos_z = std::cos(rad_z);

		bbox = object->bounding_box();

		point3 min(infinity, infinity, infinity);
		point3 max(-infinity, -infinity, -infinity);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					double x = i * bbox.x.max + (1 - i) * bbox.x.min;
					double y = j * bbox.y.max + (1 - j) * bbox.y.min;
					double z = k * bbox.z.max + (1 - k) * bbox.z.min;

					vec3 tester = transform(x, y, z);

					for (int c = 0; c < 3; c++) {
						min[c] = std::fmin(min[c], tester[c]);
						max[c] = std::fmax(max[c], tester[c]);
					}
				}
			}
		}
		bbox = aabb(min, max);
	}
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		point3 origin = point3(
			(cos_theta * r.origin().x()) - (sin_theta * r.origin().z()),
			r.origin().y(),
			(sin_theta * r.origin().x()) + (cos_theta * r.origin().z())
		);

		vec3 direction = vec3(
			(cos_theta * r.direction().x()) - (sin_theta * r.direction().z()),
			r.direction().y(),
			(sin_theta * r.direction().x()) + (cos_theta * r.direction().z())
		);

		ray rotated_r(origin, direction, r.time());

		if (!object->hit(rotated_r, ray_t, rec))
			return false;

		rec.p = point3(
			(cos_theta * rec.p.x()) + (sin_theta * rec.p.z()),
			rec.p.y(),
			(-sin_theta * rec.p.x()) + (cos_theta * rec.p.z())
		);

		rec.normal = vec3(
			(cos_theta * rec.normal.x()) + (sin_theta * rec.normal.z()),
			rec.normal.y(),
			(-sin_theta * rec.normal.x()) + (cos_theta * rec.normal.z())
		);

		return true;
	}

	aabb bounding_box() const override { return bbox; }
private:
	shared_ptr<hittable> object;
	double sin_x, sin_y, sin_z, cos_x, cos_y, cos_z;
	aabb bbox;

	vec3 transform(double x, double y, double z) {
		// rotate x
		double tmp_x = x;
		double tmp_y = cos_x * y + sin_x * z;
		double tmp_z = -sin_x * y + cos_x * z;

		// rotate y axis
		x = cos_y * tmp_x + sin_y * tmp_z;
		y = tmp_y;
		z = -sin_y * tmp_x + cos_y * tmp_z;

		// Rotate about z axis
		tmp_x = cos_z * x + sin_z * y;
		tmp_y = -sin_z * x + cos_z * y;
		tmp_z = z;

		return vec3(tmp_x, tmp_y, tmp_z);
	}

	vec3 transform(vec3& v) const {
		return transform(v.x(), v.y(), v.z());
	}

	vec3 inverse_transform(double x, double y, double z) const {
		// rotate x
		double tmp_x = x;
		double tmp_y = cos_x * y - sin_x * z;
		double tmp_z = sin_x * y + cos_x * z;

		// rotate y axis
		x = cos_y * tmp_x - sin_y * tmp_z;
		y = tmp_y;
		z = sin_y * tmp_x + cos_y * tmp_z;

		// Rotate about z axis
		tmp_x = cos_z * x - sin_z * y;
		tmp_y = sin_z * x + cos_z * y;
		tmp_z = z;

		return vec3(tmp_x, tmp_y, tmp_z);
	}

	vec3 inverse_transform(vec3& v) const {
		return inverse_transform(v.x(), v.y(), v.z());
	}
};
#endif // HITTABLE_H