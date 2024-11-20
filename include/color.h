#ifndef COLOR_H
#define COLOR_H

/**
* This file represents a color, following the Raytracing in a Weekend series by Peter Shirley
*
*/

#include "interval.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

using color = vec3;

std::ofstream fout("image.ppm");

inline double linear_to_gamma(double linear_component) {
	if (linear_component > 0)
		return std::sqrt(linear_component);
	return 0;
}

void write_color(std::ostream& out, const color& pixel_color) {
	double r = pixel_color.x();
	double g = pixel_color.y();
	double b = pixel_color.z();

	// Replace NaN components with zero
	if (r != r) r = 0.0;
	if (g != g) g = 0.0;
	if (b != b) b = 0.0;

	// Apply gamma 2 correction
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	// Translate the [0,1] component values to the byte range [0,255].
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	// Write out the pixel color components.
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

void write_image(uint8_t* image, int nx, int ny) {
	if (!fout.is_open())
	{
		std::cerr << "FILE NOT OPENED" << std::endl;
	}
	fout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; ++i) {
			fout << static_cast<int>(image[3 * (j * nx + i) + 0]) << ' ';
			fout << static_cast<int>(image[3 * (j * nx + i) + 1]) << ' ';
			fout << static_cast<int>(image[3 * (j * nx + i) + 2]) << '\n';
		}
	}
}

#endif // COLOR_H