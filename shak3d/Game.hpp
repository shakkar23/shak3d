#pragma once

#include "SDL2-Window/Window.hpp"
#include "SDL2-Inputs/inputs.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SDL2_GL/shader_compiler.hpp"
#include "Util/math.hpp"

#include <vector>
#include <array>
#include <memory>


struct VertexPN {
	GLfloat pos[3];
	GLfloat normal[3];
};

struct Model {
	GLfloat color[3];
	std::vector<VertexPN> vertices;
	std::vector<GLushort> indices;
};

struct IndexedVBO {
	std::unique_ptr<Model> group;
	GLuint vertex_buffer_ID;
	GLuint index_buffer_ID;
};


// cube
inline VertexPN vertices[] = {
	// front
	{ { -0.5f, -0.5f,  0.5f },{ 0.0f, 0.0f, 1.0f } },
	{ {  0.5f, -0.5f,  0.5f },{ 0.0f, 0.0f, 1.0f } },
	{ {  0.5f,  0.5f,  0.5f },{ 0.0f, 0.0f, 1.0f } },
	{ { -0.5f,  0.5f,  0.5f },{ 0.0f, 0.0f, 1.0f } },
	// back
	{ { -0.5f, -0.5f, -0.5f },{ 0.0f, 0.0f, -1.0f } },
	{ {  0.5f, -0.5f, -0.5f },{ 0.0f, 0.0f, -1.0f } },
	{ {  0.5f,  0.5f, -0.5f },{ 0.0f, 0.0f, -1.0f } },
	{ { -0.5f,  0.5f, -0.5f },{ 0.0f, 0.0f, -1.0f } },
	// top
	{ { -0.5f,  0.5f,  0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ {  0.5f,  0.5f,  0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ {  0.5f,  0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ { -0.5f,  0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
	// bottom
	{ { -0.5f, -0.5f,  0.5f },{ 0.0f, -1.0f, 0.0f } },
	{ {  0.5f, -0.5f,  0.5f },{ 0.0f, -1.0f, 0.0f } },
	{ {  0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f, 0.0f } },
	// right
	{ {  0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ {  0.5f, -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ {  0.5f,  0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ {  0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f } },
	// left
	{ { -0.5f, -0.5f,  0.5f },{ -1.0f, 0.0f, 0.0f } },
	{ { -0.5f, -0.5f, -0.5f },{ -1.0f, 0.0f, 0.0f } },
	{ { -0.5f,  0.5f, -0.5f },{ -1.0f, 0.0f, 0.0f } },
	{ { -0.5f,  0.5f,  0.5f },{ -1.0f, 0.0f, 0.0f } }
};

inline GLushort indices[] = {
	// front
	0, 1, 2,
	2, 3, 0,
	// back
	4, 5, 6,
	6, 7, 4,
	// top
	8, 9, 10,
	10, 11, 8,
	// bottom
	12, 13, 14,
	14, 15, 12,
	// right
	16, 17, 18,
	18, 19, 16,
	// left
	20, 21, 22,
	22, 23, 20

};


inline std::array<Model, 1> vertexGroups = { {
	{
		.color = { 1.0f, 0.0f, 0.0f },
		.vertices = std::vector<VertexPN>(vertices, vertices + sizeof(vertices) / sizeof(*vertices)),
		.indices  = std::vector<GLushort>(indices, indices + sizeof(indices) / sizeof(*indices))
	}
} };


class Game final {
	int x{}, y{};
	int texWidth, texHeight;
	// GL resources

	GLuint program_3d_id;

	GLint loc_aPos;

	SDL_Texture* texTarget = nullptr, * bmpTex = nullptr;
	SDL_Surface* bmpSurf = nullptr;


	std::vector<IndexedVBO> m_indexedVBOs;



public:
	Game(Window& window) {

		init();

		program_3d_id = compileProgram("shaders/vershak.glsl", "shaders/fragkkar.glsl");

		loc_aPos = GL_NO_CHECK(GetAttribLocation(program_3d_id, "position"));
		assert(loc_aPos >= 0);


		// Create VBOs and index buffers
		for (auto& g : vertexGroups) {
			IndexedVBO indexedVBO{};
			indexedVBO.group = std::make_unique<Model>(g);

			// Create vertex buffer
			GL_CHECK(GenBuffers(1, &indexedVBO.vertex_buffer_ID));
			GL_CHECK(BindBuffer(GL_ARRAY_BUFFER, indexedVBO.vertex_buffer_ID));
			GL_CHECK(BufferData(GL_ARRAY_BUFFER, g.vertices.size() * sizeof(VertexPN), g.vertices.data(), GL_STATIC_DRAW));

			// Create index buffer
			GL_CHECK(GenBuffers(1, &indexedVBO.index_buffer_ID));
			GL_CHECK(BindBuffer(GL_ARRAY_BUFFER, indexedVBO.index_buffer_ID));
			GL_CHECK(BufferData(GL_ARRAY_BUFFER, g.indices.size() * sizeof(GLushort), g.indices.data(), GL_STATIC_DRAW));

			m_indexedVBOs.push_back(std::move(indexedVBO));
		}


		GL_CHECK(VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
		GL_CHECK(EnableVertexAttribArray(loc_aPos));

		// Load a texture
		bmpSurf = IMG_Load("resources/texture.bmp");
		if (bmpSurf == nullptr) {
			std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
			return;
		}

		bmpTex = SDL_CreateTextureFromSurface(window.getRenderer(), bmpSurf);
		if (bmpTex == nullptr) {
			std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
			return;
		}
		auto [w, h] = window.getWindowSize();
		// Create a texture to render to
		texTarget = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

		if (texTarget == nullptr) {
			std::cerr << "SDL_CreateTexture: " << SDL_GetError() << std::endl;
			return;
		}
		texWidth = w;
		texHeight = h;

	}

	~Game() {

		//GL_CHECK(DeleteProgram(programId));
		GL_CHECK(DeleteProgram(program_3d_id));

		for (auto& indexedVBO : m_indexedVBOs) {
			GL_CHECK(DeleteBuffers(1, &indexedVBO.vertex_buffer_ID));
			GL_CHECK(DeleteBuffers(1, &indexedVBO.index_buffer_ID));
		}
	}

	// move
	Game(Game&&) noexcept = delete;
	Game& operator=(Game&&) noexcept = delete;

	//copy
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	bool update(const Shakkar::inputs& state);

	void GameRender(Window& window);

private:
	mat4x4 makeProjection(float width, float height, float fovY, float near, float far);
};

