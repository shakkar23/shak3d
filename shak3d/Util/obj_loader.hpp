#pragma once

#include "model_types.hpp"

#include <string>
#include <fstream>


inline ModelData parse(std::string path) {
	ModelData data{};

	std::ifstream f(path);

	std::vector<glm::vec3> normals; // we need this becuase the normals are not always in the file, as well as not guaranteed to be in the correct order of the vertices

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
				glm::vec3 n{};
				sscanf_s(line.c_str(), "vn %f %f %f", &n[0], &n[1], &n[2]);
				normals.push_back(n);
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

	if (normals.size() == 0)
	{
		// generate normals
		for (size_t i = 0; i < data.indices.size(); i += 3) {
			glm::vec3 v0 = data.vertices[data.indices[i + 0]].pos;
			glm::vec3 v1 = data.vertices[data.indices[i + 1]].pos;
			glm::vec3 v2 = data.vertices[data.indices[i + 2]].pos;

			glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

			data.vertices[data.indices[i + 0]].normal += normal;
			data.vertices[data.indices[i + 1]].normal += normal;
			data.vertices[data.indices[i + 2]].normal += normal;
		}

		for (auto& v : data.vertices) {
			v.normal = glm::normalize(v.normal);
		}
	}
	else
	{
		for (size_t i = 0; i < data.indices.size(); i += 3) {
			data.vertices[data.indices[i + 0]].normal = normals[data.indices[i + 0]];
			data.vertices[data.indices[i + 1]].normal = normals[data.indices[i + 1]];
			data.vertices[data.indices[i + 2]].normal = normals[data.indices[i + 2]];
		}
	}

	return data;
}