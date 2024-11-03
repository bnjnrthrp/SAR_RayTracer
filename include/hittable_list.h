#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

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
	void add(shared_ptr<hittable> object) { objects.push_back(object); }


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
	
	// Members
	hittable** list;
	int list_size;

};

#endif // HITTABLE_LIST_H