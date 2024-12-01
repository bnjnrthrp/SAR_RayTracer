/*
A simple .obj model loader for RT in a Weekend compatibility.
Original code by Drummersbrother at https://github.com/Drummersbrother/raytracing-in-one-weekend/blob/master/rtw_stb_obj_loader.h
*/

#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#define TINYOBJLOADER_IMPLEMENTATION
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#define TINYOBJLOADER_USE_DOUBLE

#include "../external/tiny_obj_loader.h"

#include <stdio.h>

#include "bvh.h"
#include "material.h"
#include "triangle.h"

color _get_color(tinyobj::real_t* raws) {
	return color(raws[0], raws[1], raws[2]);
}

shared_ptr<material> get_mtl_mat(const tinyobj::material_t& reader_mat) {
	shared_ptr<texture> diffuse_a = make_shared<solid_color>(_get_color((tinyobj::real_t*)reader_mat.diffuse));
	shared_ptr<texture> specular_a = make_shared<solid_color>(_get_color((tinyobj::real_t*)reader_mat.specular));
	shared_ptr<texture> emissive_a = make_shared<solid_color>(_get_color((tinyobj::real_t*)reader_mat.emission));
	shared_ptr<texture> transparency_a = make_shared<solid_color>(_get_color((tinyobj::real_t*)reader_mat.transmittance)*(1.0 - reader_mat.dissolve));
	shared_ptr<texture> sharpness_a = make_shared<solid_color>(color(1, 0, 0) * reader_mat.shininess);

	return make_shared<mtl_material>(
		diffuse_a,
		specular_a,
		emissive_a,
		transparency_a,
		sharpness_a,
		reader_mat.illum
	);
}

shared_ptr<hittable> load_model_from_file(std::string filename, shared_ptr<material> model_material, bool shade_smooth) {
	std::cerr << "Loading .obj file '" << filename << "'." << std::endl;

	std::string inputfile = filename;
	tinyobj::ObjReaderConfig reader_config;

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inputfile, reader_config)) {
		if (!reader.Error().empty()) {
			std::cerr << "TinyObjReader error: " << reader.Error();
		}
		exit(-1);
	}

	if (!reader.Warning().empty()) {
		std::cerr << "TinyObjReader warning: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& raw_materials = reader.GetMaterials();

	// Convert from TinyObjLoader to RT in a Weekend materials
	std::vector<shared_ptr<material>> converted_mats;
	for (auto& raw_mat : raw_materials) {
		converted_mats.push_back(get_mtl_mat(raw_mat));
	}

	const bool use_mtl_file = (raw_materials.size() != 0);

	hittable_list model_output;

	std::clog << "Made it to line 75\n";

	for (size_t s = 0; s < shapes.size(); s++) {
		hittable_list shape_triangles;

		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			const int fv = 3; 
			assert(shapes[s].mesh.num_face_vertices[f] == fv);

			vec3 tri_v[3];
			//std::clog << "Made it to line 86\n";
			vec3 tri_vn[3];
			//std::clog << "Made it to line 88\n";

			// TODO: Add a check for winding and adjust 
			
			// Loop over vertices in the face
			std::clog << "f = " << f << " of " << shapes[s].mesh.num_face_vertices.size() << "\n";
			for (size_t v = 0; v < 3; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * std::size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * std::size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * std::size_t(idx.vertex_index) + 2];
				//std::clog << "Made it to line 95\n";
				std::clog << "vx: " << vx << " vy: " << vy << " vz: " << vz << "\n";
				tri_v[v] = vec3(vx, vy, vz);

				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
					std::clog << "nx: " << nx << " ny: " << ny << " nz: " << nz << "\n";
					//std::clog << "Made it to line 102\n";
					tri_vn[v] = vec3(nx, ny, nz);
				}
				else {
					/*std::clog << "Asserting 0\n";
					assert(0);*/
					continue;
				}
			}
			//std::clog << "Made it to line 116\n";
			shared_ptr<material> tri_mat;
			if (use_mtl_file) {
				std::clog << "using mtl file\n";
				std::clog << "use mtl log " << use_mtl_file << "\n";
				tri_mat = converted_mats[shapes[s].mesh.material_ids[f]];
				std::clog << "converted mat\n";
			}
			else {
				std::clog << "Not using mtl file\n";
				tri_mat = model_material;
			}
			shape_triangles.add(make_shared<triangle>(
				tri_v[0], tri_v[1], tri_v[2], tri_mat));

			std::clog << "Assigning triangle (" << tri_v[0] << ", " << tri_v[1] << ", " << tri_v[2] << ")\n";
			std::clog << "Normals (" << tri_vn[0] << ", " << tri_vn[1] << ", " << tri_vn[2] << ")\n";
			//std::clog << "Made it to line 119\n";
			index_offset += fv;
		}
		std::clog << "Made it to line 132 - complete\n";
		model_output.add(make_shared<bvh_node>(shape_triangles, 0, 1));
	}

	return make_shared<bvh_node>(model_output, 0, 1);
}
#endif // OBJ_LOADER_H