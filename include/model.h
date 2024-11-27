#ifndef MODEL_H
#define MODEL_H

#include "triangle.h"
#include "hittable_list.h"
#include "material.h"
#include <string>

struct mesh {
	mesh(std::vector<vec3>& vert, std::vector<unsigned int>& ind) {
		vertices = vert;
		indices = ind;
	}

	std::vector<vec3> vertices;
	std::vector<unsigned int> indices;
};

class model {
public:
	model(const std::string& path, double scale = 1.0, std::shared_ptr<material> mat = make_shared<lambertian>(vec3(0, 0, 0))) : scale(scale), mat(mat)
	{
	}

private:
	std::string directory;
	std::vector<mesh> meshes;
	std::shared_ptr<material> mat;
	double scale;


	
};

#endif // MODEL_H