#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "hittable_list.h"
#include <iostream>

extern int tri_hits;
extern int tri_aabb_hits;
extern int tri_parallel;
extern int tri_intersection_behind;
extern int tri_barycentric;
extern int tri_beyond;

class triangle : public hittable {
public:
	triangle() {}
	// Constructor assumes CCW triangle winding
	triangle(vec3 v0, vec3 v1, vec3 v2, std::shared_ptr<material> mat) : v0(v0), v1(v1), v2(v2), vn0(unit_vector(cross(v1 - v0, v2-v0))), vn1(unit_vector(cross(v2 - v1, v0 - v1))), vn2(unit_vector(cross(v0 - v2, v1 - v2))), mat(mat) {
		area = cross(v1 - v0, v2 - v0).length();
		normal = unit_vector(cross(v1 - v0, v2 - v0));
		
		/*std::clog << "vertices: " << v0 << v1 << v2 << "\n";
		std::clog << "normal is " << normal << "\n";
		std::clog << "vn0 is " << vn0 << "\n";
		std::clog << "vn1 is " << vn1 << "\n";
		std::clog << "vn2 is " << vn2 << "\n";*/
		set_bounding_box();
	
		//bounding_box().print(std::clog);
	}
	triangle(vec3 v0, vec3 v1, vec3 v2, vec3 vn0, vec3 vn1, vec3 vn2, std::shared_ptr<material> mat) : v0(v0), v1(v1), v2(v2), vn0(vn0), vn1(vn1), vn2(vn2), mat(mat) {
		area = cross(v1 - v0, v2 - v0).length();
		normal = unit_vector(cross(v1 - v0, v2 - v0));
		set_bounding_box();
	}

	void setuv(vec3 uv1, vec3 uv2, vec3 uv3) {
		v0_uv = uv1;
		v1_uv = uv2;
		v2_uv = uv3;
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		tri_aabb_hits++;
		// Moller Trumbore intersection
		const float EPSILON = 1e-8;

		vec3 edge1 = v1 - v0;
		vec3 edge2 = v2 - v0;

		vec3 pvec = cross(r.direction(), edge2);
		double determinant = dot(edge1, pvec);

		double inv_determinant = 1.0 / determinant;
		vec3 tvec = r.origin() - v0;
		vec3 qvec = cross(tvec, edge1);
		// If ray is parallel to triangle, ray misses
		if (determinant > -EPSILON && determinant < EPSILON)
		{
			tri_parallel++;
			//print(std::clog, pvec, tvec, determinant);
			return false;
		}

		double u = dot(tvec, pvec) * inv_determinant;

		// Intersection is behind origin or beyond the current known intersection
		if ((u < 0.0 && std::fabs(u) > EPSILON) || (u > 1.0 && fabs(u - 1.0) > EPSILON))
		{
			tri_intersection_behind++;
			return false;
		}

		double v = dot(r.direction(), qvec) * inv_determinant;

		// Check barycentric coordinates
		if ((v < 0.0 && std::fabs(v) > EPSILON) || (v + u > 1.0 && std::fabs(u + v - 1.0) > EPSILON))
		{
			tri_barycentric++;
			return false;
		}

		double t = dot(edge2, qvec) * inv_determinant;

		// Intersection beyond current t
		if (!ray_t.contains(t))
		{
			tri_beyond++;
			return false;
		}

		// Hit
		tri_hits++;
		rec.set_face_normal(r, normal);
		rec.t = t;
		rec.p = r.at(t);
		rec.mat = mat;
		rec.u = u;
		rec.v = v;
		triangle_uv(rec.p, rec.u, rec.v);
		return true;
	}

	aabb bounding_box() const override { return bbox; }

	virtual void set_bounding_box()  {
		interval x(std::fmin(std::fmin(v0[0], v1[0]), v2[0]), std::fmax(std::fmax(v0[0], v1[0]), v2[0]));
		interval y(std::fmin(std::fmin(v0[1], v1[1]), v2[1]), std::fmax(std::fmax(v0[1], v1[1]), v2[1]));
		interval z(std::fmin(std::fmin(v0[2], v1[2]), v2[2]), std::fmax(std::fmax(v0[2], v1[2]), v2[2]));

		bbox = aabb(x, y, z);
	}

	virtual double pdf_value(const vec3& origin, const vec3& v) const override {
		hit_record rec;
		if (!this->hit(ray(origin, v), interval(0.001, infinity), rec))
			return 0.0;

		vec3 R1 = v0 - origin, R2 = v1 - origin, R3 = v2 - origin;
		double r1 = R1.length(), r2 = R2.length(), r3 = R3.length();
		double N = dot(R1, cross(R2, R3));
		double D = r1 * r2 * r3 + dot(R1, R2) * r3 + dot(R1, R3) * r2 + dot(R2, R3) * r3;

		double omega = atan2(N, D);

		return 1. / omega;
	}

	virtual vec3 random(const vec3& origin) const override {
		double r1 = random_double();
		double r2 = random_double();
		double ca = (1. - sqrt(r1)),
			cb = sqrt(r1) * (1. - r2),
			cc = r2 * sqrt(r1);
		vec3 random_in_triangle = v0 * ca + v1 * cb + v2 * cc;
		return random_in_triangle - origin;
	}

	void print(std::ostream& out) {
		out << "vertices: (" << v0 << ", " << v1 << ", " << v2 << ")" << '\n';
		out << "normal: (" << vn0 << ", " << vn1 << ", " << vn2 << ")" << '\n';
		out << "total normal: (" << normal << ")" << "\n";
	}

	void print(std::ostream& out, vec3& pvec, vec3& tvec, double determinant) const {
		// Write out the pixel color components.
		out << "vertices: (" << v0 << ", " << v1 << ", " << v2 << ")" << '\n';
		out << "normal: (" << normal[0] << ", " << normal[1] << ", " << normal[2] << ")" << '\n';
		out << "pvec: (" << pvec[0] << ", " << pvec[1] << ", " << pvec[2] << ")" << '\n';
		out << "tvec: (" << tvec[0] << ", " << tvec[1] << ", " << tvec[2] << ")" << '\n';
		out << "determinant: " << determinant << '\n';
	}

private:
	vec3 v0, v1, v2;
	vec3 vn0, vn1, vn2;
	vec3 v0_uv, v1_uv, v2_uv;
	shared_ptr<material> mat;
	double area;
	aabb bbox;
	vec3 normal;

	void triangle_uv(const vec3& p, double& u, double& v) const {
		vec3 d(u * v0_uv + v * v1_uv + (1 - u - v) * v2_uv);
		u = d[0];
		v = d[1];
	}

};

inline shared_ptr<hittable_list> tetrahedron(shared_ptr<material> mat) {
	auto sides = make_shared<hittable_list>();
	
	point3 v0(-0.5, 0.5, 0.5); // top back left
	point3 v1(0.5, 0.5, -0.5); // top front right
	point3 v2(-0.5, -0.5, -0.5); // bottom front left
	point3 v3(0.5, -0.5, 0.5); // bottom back right
	sides->add(make_shared<triangle>(v0, v1, v3, mat)); 
	sides->add(make_shared<triangle>(v1, v0, v2, mat)); 
	sides->add(make_shared<triangle>(v1, v2, v3, mat)); 
	sides->add(make_shared<triangle>(v2, v3, v0, mat)); 
	
	return sides;
}

#endif // TRIANGLE_H