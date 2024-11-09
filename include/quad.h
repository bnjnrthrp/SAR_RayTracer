#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"

class quad : public hittable {
public: 
	quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat) : Q(Q), u(u), v(v), mat(mat) 
	{ 
		vec3 n = cross(u, v);
		normal = unit_vector(n);
		D = dot(normal, Q);
		w = n / dot(n, n);
			
		set_bounding_box(); 
	}

	virtual void set_bounding_box() {
		aabb bbox_diagonal1 = aabb(Q, Q + u + v);
		aabb bbox_diagonal2 = aabb(Q + u, Q + v);
		bbox = aabb(bbox_diagonal1, bbox_diagonal2);
	}

	aabb bounding_box() const override { return bbox; }

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override { 
		double denom = dot(normal, r.direction());

		// If ray is parallel to plane, it misses
		if (std::fabs(denom) < 1e-8)
			return false;

		// Return false if hit point parameter t is outside ray interval
		double t = (D - dot(normal, r.origin())) / denom;
		if (!ray_t.contains(t))
			return false;

		point3 intersection = r.at(t);
		vec3 planar_hitpt_vector = intersection - Q;
		double alpha = dot(w, cross(planar_hitpt_vector, v));
		double beta = dot(w, cross(u, planar_hitpt_vector));

		if (!is_interior(alpha, beta, rec))
			return false;

		rec.t = t;
		rec.p = intersection;
		rec.mat = mat;
		rec.set_face_normal(r, normal);

		return true;
	}

	virtual bool is_interior(double a, double b, hit_record& rec) const {
		interval unit_interval = interval(0, 1);

		if (!unit_interval.contains(a) || !unit_interval.contains(b))
			return false;

		rec.u = a;
		rec.v = b;
		return true;
	}

private:
	point3 Q;
	vec3 u, v;
	vec3 w;
	shared_ptr<material> mat;
	aabb bbox;
	vec3 normal;
	double D;
};

#endif // QUAD_H