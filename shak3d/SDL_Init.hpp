#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

struct SDL2_init {
	SDL2_init() {
		SDL_Init(SDL_INIT_EVERYTHING | SDL_VIDEO_OPENGL);
		// initialize SDL_ttf
		TTF_Init();
	}
	~SDL2_init() {
		SDL_Quit();
	}
};
