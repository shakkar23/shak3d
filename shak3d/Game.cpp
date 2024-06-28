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

	if(state.getKey(SDLK_w).held)
	{
		camera_position[2] += 0.1f;
	}

	if (state.getKey(SDLK_s).held)
	{
		camera_position[2] -= 0.1f;
	}
	
	if (state.getKey(SDLK_d).held)
	{
		camera_position[0] -= 0.1f;
	}

	if (state.getKey(SDLK_a).held)
	{
		camera_position[0] += 0.1f;
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
		pitch++;
	}

	if (state.getKey(SDLK_DOWN).held)
	{
		pitch--;
	}

	constexpr float dt = 1.0 / UPDATES_PER_SECOND;
	theta += dt;


	return true;
}


mat4x4 Game::makeProjection(float width, float height, float fovY, float near, float far) {
	// convert fovY from degrees to radians
	fovY = fovY * 3.14159265358979323846f / 180.0f;

	mat4x4 result{};
	float ooZRange = 1 / (near - far);
	float linearHalfFovy = tan(fovY * 0.5f);
	float linearHalfFovx = linearHalfFovy * width / height;
	result[0][0] = 1.f / linearHalfFovx;
	result[1][1] = 1.f / linearHalfFovy;
	result[2][2] = (near + far) * ooZRange;
	result[2][3] = -1.f;
	result[3][2] = (2 * near * far) * ooZRange;
	return result;
}


void Game::GameRender(Window& window)
{
	window.setDrawColor(150, 150, 150, 255);
	window.clear();

	auto [win_x, win_y] = window.getWindowSize();

	GL_CHECK(Viewport(0, 0, win_x, win_y));

	if (win_x != texWidth || win_y != texHeight)
	{
		SDL_DestroyTexture(texTarget);
		texTarget = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win_x, win_y);
		texWidth = win_x;
		texHeight = win_y;

		auto proj = makeProjection(win_x, win_y, 90.0f, 0.01f, 100.0f);
		
		GL_CHECK(UseProgram(program_3d_id));
		if(loc_uProjCameraToView >= 0)
			GL_CHECK(UniformMatrix4fv(loc_uProjCameraToView, 1, GL_FALSE, (*proj.data.data()).data.data()));

	}


	GL_CHECK(UseProgram(program_3d_id));


	// Set the matrix
	{
		auto to_rad = [](float deg) {return deg * 3.14159265358979323846f / 180.0f; };
		mat4x4 matRotZ = mat4x4::rotate_z(0);
		mat4x4 matRotX = mat4x4::rotate_x(0 * 0.5f);
		mat4x4 matTrans = mat4x4::identity();
		matTrans[3][0] = camera_position[0];
		matTrans[3][1] = camera_position[1];
		matTrans[3][2] = camera_position[2];

		vec<2> eulerAngles{ to_rad(yaw),to_rad(pitch) };
		
		// default direction the camera is facing with an extra 1 for matmuls
		vec<4> target = { 0,0,1,1 };

		mat4x4 camera_rot_x = mat4x4::rotate_x(eulerAngles[1]);
		mat4x4 camera_rot_y = mat4x4::rotate_y(eulerAngles[0]);

		target = camera_rot_x*camera_rot_y * target;

		// calulate the view matrix using the lookAt function (eulerToVec)
		vec<3> lookAt = camera_position + target.dehomogenize();

		mat4x4 matView =  mat4x4::lookAt(camera_position, lookAt, { 0,1,0 }).inverse();
		
		if(loc_uProjWorldToCamera >= 0)
			GL_CHECK(UniformMatrix4fv(loc_uProjWorldToCamera, 1, GL_FALSE, (*matView.data.data()).data.data()));

		// combine all the matrices that we can
		mat4x4 matWorld = matRotX * matRotZ * matTrans;

		if(loc_uProjModelToWorld >= 0)
			GL_CHECK(UniformMatrix4fv(loc_uProjModelToWorld, 1, GL_FALSE, (*matWorld.data.data()).data.data()));

	}


	// Iterate over Models
	for (auto& indexedVBO : models) {
		// Set remaining uniforms and vertex attributes

		GL_CHECK(BindBuffer(GL_ARRAY_BUFFER, indexedVBO.vertex_buffer_ID));
		if (loc_aPos >= 0) {
			GL_CHECK(VertexAttribPointer(loc_aPos, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(VertexPN), (GLvoid*)offsetof(VertexPN, pos)));
			GL_CHECK(EnableVertexAttribArray(loc_aPos));
		}

		// Draw this VBO
		GL_CHECK(BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexedVBO.index_buffer_ID));
		GL_CHECK(DrawElements(GL_TRIANGLES, (GLsizei)indexedVBO.model->indices.size(), GL_UNSIGNED_SHORT, (void*)0));
	}

	window.display();
}