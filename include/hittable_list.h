#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "aabb.h"
#include "hittable.h"

class hittable_list : public hittable {
public:
	// Public Members
	std::vector<shared_ptr<hittable>> objects;

	// Constructors
	hittable_list() {}
	hittable_list(shared_ptr<hittable> object) { add(object); }

	// Methods
	void clear() { objects.clear(); }
	void add(shared_ptr<hittable> object) { 
		objects.push_back(object); 

		/*std::clog << "Combining bboxes: ";
		bbox.print(std::clog);
		object->bounding_box().print(std::clog);*/

		bbox = aabb(bbox, object->bounding_box());
	}


	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		hit_record tmp_rec;
		bool hit_anything = false;
		double closest_so_far = ray_t.max;

		for (const auto& object : objects) {
			if (object->hit(r, interval(ray_t.min, closest_so_far), tmp_rec)) {
				hit_anything = true;
				closest_so_far = tmp_rec.t;
				rec = tmp_rec;
			}
		}
		return hit_anything;
	}
	
	aabb bounding_box() const override { return bbox; }

	double pdf_value(const point3& origin, const vec3& direction) const override {
		double weight = 1.0 / objects.size();
		double sum = 0.0;

		for (const auto& object : objects)
			sum += weight * object->pdf_value(origin, direction);

		return sum;
	}

	vec3 random(const point3& origin) const override {
		int int_size = int(objects.size());
		return objects[random_int(0, int_size - 1)]->random(origin);
	}

private:
	// Members
	aabb bbox;
	
};

#endif // HITTABLE_LIST_H