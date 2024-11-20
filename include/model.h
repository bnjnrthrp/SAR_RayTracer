#ifndef MODEL_H
#define MODEL_H

#include "../external/assimp/Importer.hpp"
#include "../external/assimp/scene.h"
#include "../external/assimp/postprocess.h"
#include "triangle.h"
#include "hittable_list.h"
#include "material.h"
#include <string>

struct mesh {
	mesh(std::vector<vec3>& vert, std::vector<int>& ind) {
		vertices = vert;
		indices = ind;
	}

	std::vector<vec3> vertices;
	std::vector<int> indices;
};

class model {
public:
	model(const std::string& path, double scale = 1.0, std::shared_ptr<material> mat = make_shared<lambertian>(vec3(0, 0, 0))) : scale(scale), mat(mat)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cerr << "Error::Assimp:: " << importer.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		process_node(scene->mRootNode, scene);
	}

private:
	std::string directory;
	std::vector<mesh> meshes;
	std::shared_ptr<material> mat;
	double scale;


	void process_node(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			process_mesh(mesh, scene);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			process_node(node->mChildren[i], scene);
		}
	}

	void process_mesh(aiMesh* aiMesh, const aiScene* scene) {
		std::vector<vec3> vertices;
		std::vector<int> indices;

		for (unsigned int i = 0; i < aiMesh->mNumVertices; i++) {
			vec3 pos((double)aiMesh->mVertices[i].x,
				(double)aiMesh->mVertices[i].y,
				-(double)aiMesh->mVertices[i].z);

				vertices.push_back(pos);
		}

		for (unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
			aiFace face = aiMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) 
				indices.push_back(face.mIndices[j]);
		}
		meshes.push_back(mesh(vertices, indices));
	}
};

#endif // MODEL_H