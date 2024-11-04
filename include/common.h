#ifndef COMMON_H
#define COMMON_H

/*
* Utility file for Ray Tracing in a Weekend series - contains some basic constants and features such as random values
*/

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

/*Returns a random real in range [0, 1)*/
inline double random_double() {
	return std::rand() / (RAND_MAX + 1.0);
}

/*Returns a random real in range [min, max)*/
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}

/*Returns a random integer in [min,max].*/
inline int random_int(int min, int max) {
	return int(random_double(min, max + 1));
}

// Common headers

#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif // COMMON_H