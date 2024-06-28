#pragma once

#include "model_types.hpp"

#include <string>
#include <fstream>


inline ModelData parse(std::string path) {
	ModelData data{};

	std::ifstream f(path);

	std::string line;
	while (std::getline(f, line)) {
		if (line[0] == 'v') {
			if (line[1] == ' ') {
				// vertex
				VertexPN v{};
				sscanf_s(line.c_str(), "v %f %f %f", &v.pos[0], &v.pos[1], &v.pos[2]);
				data.vertices.push_back(v);
			}
			else if (line[1] == 'n') {
				// normal TODO
				/*vec<3> n;
				sscanf(line.c_str(), "vn %f %f %f", &n[0], &n[1], &n[2]);
				data.normals.push_back(n);*/
			}
		}
		else if (line[0] == 'f') {
			// face
			GLushort v[3]{};
			sscanf_s(line.c_str(), "f %hu %hu %hu", &v[0], &v[1], &v[2]);
			data.indices.push_back(v[0] - 1);
			data.indices.push_back(v[1] - 1);
			data.indices.push_back(v[2] - 1);
		}
	}

	return data;
}