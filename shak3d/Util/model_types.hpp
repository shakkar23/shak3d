#pragma once 

#include "math.hpp"

#include <vector>
#include <memory>

#include "SDL2/SDL_opengles2_khrplatform.h"
#include "SDL2/SDL_opengles2_gl2platform.h"
#include "SDL2/SDL_opengles2_gl2.h"
#include "SDL2/SDL_opengles2_gl2ext.h"


struct VertexPN {
	vec<3> pos;
	vec<3> normal;
};

struct ModelData {
	GLfloat color[3];
	std::vector<VertexPN> vertices;
	std::vector<GLushort> indices;
};

struct Model {
	std::unique_ptr<ModelData> model;
	GLuint vertex_buffer_ID;
	GLuint index_buffer_ID;
};
