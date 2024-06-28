#pragma once

#include "SDL2-Window/Window.hpp"
#include "SDL2-Inputs/inputs.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SDL2_GL/shader_compiler.hpp"
#include "Util/math.hpp"
#include "Util/model_types.hpp"
#include "Util/obj_loader.hpp"

#include <vector>
#include <array>
#include <memory>




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


class Game final {
	float theta = 0.0f;
	int x{}, y{};
	int yaw{}, pitch{};
	int texWidth, texHeight;
	// GL resources

	GLuint program_3d_id;

	GLint loc_aPos;
	GLint loc_uProjCameraToView;
	GLint loc_uProjModelToWorld;
	GLint loc_uProjWorldToCamera;

	SDL_Texture* texTarget = nullptr, * bmpTex = nullptr;
	SDL_Surface* bmpSurf = nullptr;
	vec<3> camera_position{ 0,0,-1 };


	std::vector<Model> models;



public:
	Game(Window& window) {

		init();

		program_3d_id = compileProgram("shaders/vershak.glsl", "shaders/fragkkar.glsl");

		loc_aPos = GL_NO_CHECK(GetAttribLocation(program_3d_id, "position"));
		assert(loc_aPos >= 0);

		std::vector<ModelData> vertexGroups;
		// load horse.obj
		vertexGroups.push_back(parse("assets/horse.obj"));

		// Create VBOs and index buffers
		for (auto& g : vertexGroups) {
			Model indexedVBO{};
			indexedVBO.model = std::make_unique<ModelData>(g);

			// Create vertex buffer
			GL_CHECK(GenBuffers(1, &indexedVBO.vertex_buffer_ID));
			GL_CHECK(BindBuffer(GL_ARRAY_BUFFER, indexedVBO.vertex_buffer_ID));
			GL_CHECK(BufferData(GL_ARRAY_BUFFER, g.vertices.size() * sizeof(VertexPN), g.vertices.data(), GL_STATIC_DRAW));

			// Create index buffer
			GL_CHECK(GenBuffers(1, &indexedVBO.index_buffer_ID));
			GL_CHECK(BindBuffer(GL_ARRAY_BUFFER, indexedVBO.index_buffer_ID));
			GL_CHECK(BufferData(GL_ARRAY_BUFFER, g.indices.size() * sizeof(GLushort), g.indices.data(), GL_STATIC_DRAW));

			models.push_back(std::move(indexedVBO));
		}


		GL_CHECK(VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));
		GL_CHECK(EnableVertexAttribArray(loc_aPos));


		auto [w, h] = window.getWindowSize();


		// Get uniform locations
			loc_uProjCameraToView = GL_NO_CHECK(GetUniformLocation(program_3d_id, "uProjCameraToView"));

			loc_uProjModelToWorld = GL_NO_CHECK(GetUniformLocation(program_3d_id, "uProjModelToWorld"));

			loc_uProjWorldToCamera = GL_NO_CHECK(GetUniformLocation(program_3d_id, "uProjWorldToCamera"));

		// Set projection matrix
			auto proj = makeProjection(w, h, 90.0f, 0.01f, 100.0f);

			GL_CHECK(UseProgram(program_3d_id));
			GL_CHECK(UniformMatrix4fv(loc_uProjCameraToView, 1, GL_FALSE, (*proj.data.data()).data.data()));


		// Create a texture
		texTarget = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);

		// Load a texture
		bmpSurf = IMG_Load("assets/image.png");
		if (bmpSurf == nullptr) {
			std::cerr << "IMG_Load: " << IMG_GetError() << std::endl;
			return;
		}

		bmpTex = SDL_CreateTextureFromSurface(window.getRenderer(), bmpSurf);
		if (bmpTex == nullptr) {
			std::cerr << "SDL_CreateTextureFromSurface: " << SDL_GetError() << std::endl;
			return;
		}


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

		for (auto& indexedVBO : models) {
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

