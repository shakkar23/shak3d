#include "Game.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <array>


bool Game::update(const Shakkar::inputs& state)
{
	if (state.getMouse().left_held)
	{
		x = state.getMouse().x;
		y = state.getMouse().y;
	}
	return true;
}


mat4x4 Game::makeProjection(float width, float height, float fovY, float near, float far) {
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
	}


	GL_CHECK(UseProgram(program_3d_id));

	// Iterate over VBOs
	for (size_t i = 0; i < m_indexedVBOs.size(); i++) {

		// Set remaining uniforms and vertex attributes
		const IndexedVBO& indexedVBO = m_indexedVBOs[i];

		GL_CHECK(BindBuffer(GL_ARRAY_BUFFER, indexedVBO.vertex_buffer_ID));

		GL_CHECK(VertexAttribPointer(loc_aPos, 3, GL_FLOAT, GL_FALSE, (GLsizei)sizeof(VertexPN), (GLvoid*)offsetof(VertexPN, pos)));
		GL_CHECK(EnableVertexAttribArray(loc_aPos));

		// Draw this VBO
		GL_CHECK(BindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexedVBO.index_buffer_ID));
		GL_CHECK(DrawElements(GL_TRIANGLES, (GLsizei)indexedVBO.group->indices.size(), GL_UNSIGNED_SHORT, (void*)0));
	}

	window.display();
}