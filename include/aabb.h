#ifndef AABB_H
#define AABB_H

#include "../external/tiny_obj_loader.h"

class aabb {
public:
	interval x, y, z;

	aabb() {}

	aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) { pad_to_minimums(); }

	aabb(const point3& a, const point3& b) {
		x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
		y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
		z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

		pad_to_minimums();
	}

	aabb(const aabb& box0, const aabb& box1) {
		x = interval(box0.x, box1.x);
		y = interval(box0.y, box1.y);
		z = interval(box0.z, box1.z);
	}

	aabb(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attrib) {
		point3 v_min(infinity, infinity, infinity);
		point3 v_max(-infinity, -infinity, -infinity);
		for (size_t s = 0; s < shapes.size(); s++) {
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				// Iterate through each triangle and pull the max component of each one
				for (size_t v = 0; v < 3; v++) {
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					double vx = attrib.vertices[3 * std::size_t(idx.vertex_index) + 0];
					double vy = attrib.vertices[3 * std::size_t(idx.vertex_index) + 1];
					double vz = attrib.vertices[3 * std::size_t(idx.vertex_index) + 2];

					v_min = point3(std::fmin(v_min.x(), vx), std::fmin(v_min.y(), vy), std::fmin(v_min.z(), vz));
					v_max = point3(std::fmax(v_max.x(), vx), std::fmax(v_max.y(), vy), std::fmax(v_max.z(), vz));
				} 
			}
		}
	
		aabb bbox(v_min, v_max);
		x = bbox.x;
		y = bbox.y;
		z = bbox.z;

		pad_to_minimums();
	}

	const interval& axis_interval(int n) const {
		if (n == 1) return y;
		if (n == 2) return z;
		return x;
	}

	bool hit(const ray& r, interval ray_t) const {
		const point3& ray_orig = r.origin();
		const vec3& ray_dir = r.direction();

		for (int axis = 0; axis < 3; axis++) {
			const interval& ax = axis_interval(axis);
			const double adinv = 1.0 / ray_dir[axis];

			double t0 = (ax.min - ray_orig[axis]) * adinv;
			double t1 = (ax.max - ray_orig[axis]) * adinv;

			if (t0 < t1) {
				if (t0 > ray_t.min) ray_t.min = t0;
				if (t1 < ray_t.min) ray_t.max = t1;
			}
			else {
				if (t1 > ray_t.min) ray_t.min = t1;
				if (t0 < ray_t.max) ray_t.max = t0;
			}

			if (ray_t.max <= ray_t.min)
				return false;
		}
		return true;
	}

	int longest_axis() const {
		// Returns the index of the longest axis of the bounding box.

		if (x.size() > y.size())
			return x.size() > z.size() ? 0 : 2;
		else
			return y.size() > z.size() ? 1 : 2;
	}

	void print(const std::ostream&) {
		point3 min = point3(x.min, y.min, z.min);
		point3 max = point3(x.max, y.max, z.max);

		std::clog << "Bounding box: min (" << min << ") - (" << max << ")\n";
	}

	point3 get_center() const {
		return point3(x.midpoint(), y.midpoint(), z.midpoint());
	}

	static const aabb empty, universe;

private:
	void pad_to_minimums() {
		double delta = 0.0001;
		if (x.size() < delta) x = x.expand(delta);
		if (y.size() < delta) y = y.expand(delta);
		if (z.size() < delta) z = z.expand(delta);
	}
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

aabb operator+(const aabb& bbox, const vec3& offset) {
	return aabb(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
}

aabb operator+(const vec3 offset, const aabb& bbox) {
	return bbox + offset;
}

aabb operator*(const aabb& bbox, const vec3& scale) {
	return aabb(bbox.x + scale.x(), bbox.y + scale.y(), bbox.z + scale.z());
}

aabb operator*(const vec3& scale, const aabb& bbox) {
	return bbox * scale;
}

#endif // AABB_H