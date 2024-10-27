// SAR_RayTracer.cpp : Defines the entry point for the application.
//

#include "../include/SAR_RayTracer.h"
#include "../include/vec3.h"
#include <iostream>

int main()
{
	int nx, ny;

	nx = 200;
	ny = 100;
	std::cout << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(double(i) / double(nx), double(j) / double(ny), 0.2);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}
