#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include <map>

enum SPECTRUM {VISIBLE, X, C, L};

// Size of the spectrum enum
//static double SPECTRAL_ARRAY[4] = { 5.5e-7, .03, .06, .23 };

static std::map<SPECTRUM, double> SPECTRAL_MAP{
	{VISIBLE, 5.5e-7},
	{X, .03},
	{C, .06},
	{L, .23}
};

class ray
{
public:
	ray() {}



	ray(const point3& origin, const vec3& direction, double time) : orig(origin), dir(direction), tm(time) {}
	ray(const point3& origin, const vec3& direction) : ray(origin, direction, 0) {}

	// Getters
	const point3& origin() const { return orig; }
	const vec3& direction() const { return dir; }
	double time() const { return tm; }
	
	point3 at(double t) const { return orig + t * dir; }

private:
	// Member variables
	point3 orig;
	vec3 dir;
	double tm;
};

#endif // RAY_H