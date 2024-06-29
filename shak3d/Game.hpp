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



class Game final {
	float theta = 0.0f;
	// player information
	int x{}, y{};
	int yaw{}, pitch{};
	glm::vec3 camera_position{ 0,0,-1 };
	
	// window information
	int win_width, win_height;

	// GL resources
	GLuint program_3d_id;

	GLint loc_aPos;
	GLint loc_uProjCameraToView;
	GLint loc_uProjModelToWorld;
	GLint loc_uProjWorldToCamera;
	GLint loc_light_dir;
	GLint loc_normal;
	
	// SDL resources
	SDL_Texture* texTarget = nullptr, * bmpTex = nullptr;
	SDL_Surface* bmpSurf = nullptr;


	std::vector<Model> models;

	// world information
	glm::vec3 light_dir = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));


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

			// Create vertex normal buffer
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

			loc_light_dir = GL_NO_CHECK(GetUniformLocation(program_3d_id, "light_dir"));

			loc_normal = GL_NO_CHECK(GetAttribLocation(program_3d_id, "normal"));


		// Set projection matrix
			auto proj = glm::perspective(90.0f, (float(w) / h) , 0.01f, 100.0f);

			GL_CHECK(UseProgram(program_3d_id));

			GL_CHECK(UniformMatrix4fv(loc_uProjCameraToView, 1, GL_FALSE, glm::value_ptr(proj)));


		// Set light direction
			GL_CHECK(Uniform3f(loc_light_dir, light_dir.x, light_dir.y, light_dir.z));


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


		win_width = w;
		win_height = h;

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
};

