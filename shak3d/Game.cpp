#include "Game.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Util/obj_loader.hpp"

#include <array>


bool Game::update(const Shakkar::inputs& state)
{
	if (state.getMouse().left_held)
	{
		x = state.getMouse().x;
		y = state.getMouse().y;
	}

	bool faster = false;
	if(state.getKey(SDLK_LSHIFT).held)
	{
		faster = true;
	}

	const float speed = 0.00001f * (faster * 60 + 1);

	if(state.getKey(SDLK_w).held)
	{
		// find the direction the camera is facing
		auto to_rad = [](float deg) {return deg * 3.14159265358979323846f / 180.0f; };
		glm::vec2 eulerAngles{ to_rad(yaw),to_rad(pitch) };
		glm::vec4 target = { 0,0,1,1 };

		glm::mat4x4 camera_rot_x = glm::rotate(glm::mat4x4(1.0f), eulerAngles[1], glm::vec3(1, 0, 0));
		glm::mat4x4 camera_rot_y = glm::rotate(glm::mat4x4(1.0f), eulerAngles[0], glm::vec3(0, 1, 0));

		target = camera_rot_y * camera_rot_x * target;

		// move the camera in the direction it is facing
		camera_position = camera_position + glm::vec3(target) * (speed * 6);
	}

	if (state.getKey(SDLK_s).held)
	{
		// find the direction the camera is facing
		auto to_rad = [](float deg) {return deg * 3.14159265358979323846f / 180.0f; };
		glm::vec2 eulerAngles{ to_rad(yaw),to_rad(pitch) };
		glm::vec4 target = { 0,0,1,1 };

		glm::mat4x4 camera_rot_x = glm::rotate(glm::mat4x4(1.0f), eulerAngles[1], glm::vec3(1, 0, 0));
		glm::mat4x4 camera_rot_y = glm::rotate(glm::mat4x4(1.0f), eulerAngles[0], glm::vec3(0, 1, 0));

		target = camera_rot_y * camera_rot_x * target;

		// move the camera in the direction it is facing
		camera_position = camera_position - glm::vec3(target) * (speed * 6);
	}

	if (state.getKey(SDLK_d).held)
	{
		// find the direction the camera is facing
		auto to_rad = [](float deg) {return deg * 3.14159265358979323846f / 180.0f; };
		glm::vec2 eulerAngles{ to_rad(yaw),to_rad(pitch) };
		glm::vec4 target = { 0,0,1,1 };

		glm::mat4x4 camera_rot_x = glm::rotate(glm::mat4x4(1.0f), eulerAngles[1], glm::vec3(1, 0, 0));
		glm::mat4x4 camera_rot_y = glm::rotate(glm::mat4x4(1.0f), eulerAngles[0], glm::vec3(0, 1, 0));

		target = camera_rot_y * camera_rot_x * target;

		// move the camera to the right of the direction it is facing
		camera_position = camera_position + glm::vec3(glm::cross(glm::vec3(target), glm::vec3(0, 1, 0)) * speed);
	}

	if (state.getKey(SDLK_a).held)
	{
		// find the direction the camera is facing
		auto to_rad = [](float deg) {return deg * 3.14159265358979323846f / 180.0f; };
		glm::vec2 eulerAngles{ to_rad(yaw),to_rad(pitch) };
		glm::vec4 target = { 0,0,1,1 };

		glm::mat4x4 camera_rot_x = glm::rotate(glm::mat4x4(1.0f), eulerAngles[1], glm::vec3(1, 0, 0));
		glm::mat4x4 camera_rot_y = glm::rotate(glm::mat4x4(1.0f), eulerAngles[0], glm::vec3(0, 1, 0));

		target = camera_rot_y * camera_rot_x * target;

		// move the camera to the right of the direction it is facing
		camera_position = camera_position - glm::vec3(glm::cross(glm::vec3(target), glm::vec3(0, 1, 0)) * speed);
	}

	if (state.getKey(SDLK_LEFT).held)
	{
		 yaw++;
	}

	if (state.getKey(SDLK_RIGHT).held)
	{
		yaw--;
	}

	if (state.getKey(SDLK_UP).held)
	{
		pitch--;
	}

	if (state.getKey(SDLK_DOWN).held)
	{
		pitch++;
	}

	constexpr float dt = 1.0 / UPDATES_PER_SECOND;
	theta += dt;


	return true;
}


void Game::GameRender(Window& window)
{

    // Clear viewport and set background color
	window.setDrawColor(150, 150, 150, 255);
	window.clear();
	GL_CHECK(Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	// Disable alpha blend
	GL_CHECK(Disable(GL_BLEND));

	// Enable depth test
	GL_CHECK(Enable(GL_DEPTH_TEST));
	GL_CHECK(DepthMask(GL_TRUE));

	// Enable face culling
	GL_CHECK(Enable(GL_CULL_FACE));
	GL_CHECK(CullFace(GL_BACK));
	GL_CHECK(FrontFace(GL_CCW));

	auto [win_x, win_y] = window.getWindowSize();

	GL_CHECK(Viewport(0, 0, win_x, win_y));

	if (win_x != win_width || win_y != win_height)
	{
		SDL_DestroyTexture(texTarget);
		texTarget = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win_x, win_y);
		win_width = win_x;
		win_height = win_y;

		auto proj = glm::perspective(90.0f, (float(win_x) / win_y), 0.01f, 100.0f);
		
		GL_CHECK(UseProgram(program_3d_id));
		if(loc_uProjCameraToView >= 0)
			GL_CHECK(UniformMatrix4fv(loc_uProjCameraToView, 1, GL_FALSE, glm::value_ptr(proj)));

	}


	GL_CHECK(UseProgram(program_3d_id));


	// Set the matrix
	{
		auto to_rad = [](float deg) {return float(deg * M_PI  / 180.0f); };

		glm::mat4x4 matWorld = glm::translate(glm::mat4x4(1.0f), glm::vec3(0));
		matWorld = glm::rotate(matWorld, 0.f, glm::vec3(0, 0, 1));
		matWorld = glm::rotate(matWorld, 0.f * 0.5f, glm::vec3(1, 0, 0));

		// default direction the camera is facing with an extra 1 for matmuls or something
		const glm::vec4 target = { 0,0,1,1 };

		// given a yaw and a pitch in degrees assuming the default direction is 0,0,1, we can rotate the target vector
		// to get the direction the camera is facing
		glm::mat4x4 camera_rot_x = glm::rotate(glm::mat4x4(1.0f), glm::radians(float(pitch)), glm::vec3(1, 0, 0));
		glm::mat4x4 camera_rot_y = glm::rotate(glm::mat4x4(1.0f), glm::radians(float(yaw)), glm::vec3(0, 1, 0));

		glm::vec3 rotated_target = camera_rot_y * camera_rot_x * target;

		// calulate the view matrix using the lookAt function (eulerToVec)
		glm::vec3 lookAt = camera_position + rotated_target;

		glm::mat4x4 matView = glm::lookAt(camera_position, lookAt, glm::vec3(0, 1, 0));

		if(loc_uProjWorldToCamera >= 0)
			GL_CHECK(UniformMatrix4fv(loc_uProjWorldToCamera, 1, GL_FALSE, glm::value_ptr(matView)));

		if(loc_uProjModelToWorld >= 0)
			GL_CHECK(UniformMatrix4fv(loc_uProjModelToWorld, 1, GL_FALSE, glm::value_ptr(matWorld)));
	}


	// Iterate over Models
	for (auto& indexedVBO : models) {
		// Set remaining uniforms and vertex attributes

		GL_CHECK(BindBuffer(GL_ARRAY_BUFFER, indexedVBO.vertex_buffer_ID));
		if (loc_aPos >= 0) {
			GL_CHECK(VertexAttribPointer(loc_aPos, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(VertexPN), (GLvoid*)offsetof(VertexPN, pos)));
			GL_CHECK(EnableVertexAttribArray(loc_aPos));
		}

		// set normal attribute
		if (loc_normal >= 0) {
			GL_CHECK(VertexAttribPointer(loc_normal, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(VertexPN), (GLvoid*)offsetof(VertexPN, normal)));
			GL_CHECK(EnableVertexAttribArray(loc_normal));
		}

		// Draw this VBO
		GL_CHECK(BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexedVBO.index_buffer_ID));
		GL_CHECK(DrawElements(GL_TRIANGLES, (GLsizei)indexedVBO.model->indices.size(), GL_UNSIGNED_SHORT, (void*)0));
	}

	window.display();
}