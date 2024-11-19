#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "hittable_list.h"

class triangle : public hittable {
public:
	// Constructor assumes CCW triangle winding
	triangle(vec3 a, vec3 b, vec3 c, std::shared_ptr<material> mat) : a(a), b(b), c(c), mat(mat) {
		area = cross(b - a, c - a).length();
		normal = unit_vector(cross(b - a, c - a));
	}
	void setuv(vec3 uv1, vec3 uv2, vec3 uv3) {
		a_uv = uv1;
		b_uv = uv2;
		c_uv = uv3;
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		// Moller Trumbore intersection
		const float EPSILON = 1e-8;

		vec3 edge1 = b - a;
		vec3 edge2 = c - a;
		vec3 ray_cross_edge2 = cross(r.direction(), edge2);
		double determinant = dot(edge1, ray_cross_edge2);

		// If ray is parallel to triangle, ray misses
		if (fabs(determinant) < EPSILON)
			return false;

		double inv_determinant = 1.0 / determinant;
		vec3 triangle_ray = r.origin() - a;
		double u = inv_determinant * dot(triangle_ray, ray_cross_edge2);

		// Intersection is behind origin or beyond the current known intersection
		if ((u < 0.0 && std::fabs(u) > EPSILON) || (u > 1.0 && fabs(u - 1.0) > EPSILON))
			return false;

		vec3 triangle_ray_cross_edge1 = cross(triangle_ray, edge1);
		double v = inv_determinant * dot(r.direction(), triangle_ray_cross_edge1);

		// Check barycentric coordinates
		if ((v < 0.0 && std::fabs(v) > EPSILON) || (v + u > 1.0 && fabs(u + v - 1.0) > EPSILON))
			return false;

		double t = inv_determinant * dot(edge2, triangle_ray_cross_edge1);

		// Intersection beyond current t
		if (!ray_t.contains(t))
			return false;

		// Hit
		rec.set_face_normal(r, normal);
		rec.t = t;
		rec.p = r.at(t);
		rec.mat = mat;
		rec.u = u;
		rec.v = v;
		triangle_uv(rec.p, rec.u, rec.v);
		return true;
	}

	aabb bounding_box() const override {
		interval x(std::fmin(std::fmin(a[0], b[0]), c[0]), std::fmax(std::fmax(a[0], b[0]), c[0]));
		interval y(std::fmin(std::fmin(a[1], b[1]), c[1]), std::fmax(std::fmax(a[1], b[1]), c[1]));
		interval z(std::fmin(std::fmin(a[2], b[2]), c[2]), std::fmax(std::fmax(a[2], b[2]), c[2]));
		return aabb(x, y, z);
	}

	virtual double pdf_value(const vec3& origin, const vec3& v) const override {
		hit_record rec;
		if (!this->hit(ray(origin, v), interval(0.001, infinity), rec))
			return 0.0;

		double distance_squared = rec.t * rec.t * v.length_squared();
		double cosine = fabs(dot(v, rec.normal) / v.length());

		return distance_squared / (cosine * area);
	}

	virtual vec3 random(const vec3& origin) const override {
		double r1 = random_double();
		double r2 = random_double() * r1;
		vec3 random_point = a + r1 * (b - a) + r2 * (c - b);
		return random_point - origin;
	}

private:
	vec3 a, b, c;
	vec3 a_uv, b_uv, c_uv;
	shared_ptr<material> mat;
	double area;
	aabb bbox;
	vec3 normal;

	void triangle_uv(const vec3& p, double& u, double& v) const {
		vec3 d(u * a_uv + v * b_uv + (1 - u - v) * c_uv);
		u = d[0];
		v = d[1];
	}
};

#endif // TRIANGLE_H