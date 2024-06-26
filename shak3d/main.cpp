
#define WINDOW_IMPL
#include "SDL2-Window/Window.hpp"
#define SHADER_COMPILER_IMPL
#include "SDL2_GL/shader_compiler.hpp"

#include "SDL_Init.hpp"
#include "Game.hpp"
#include "enum_to_str.hpp"
#include "SDL2-Inputs/inputs.hpp"


void ProcessInputs(SDL_Event& event, bool& shouldDisplay, Shakkar::inputs& input, bool& gameRunning);


int main(int argc, char *argv[]) {
	SDL2_init sdl2_init;

	Window window("SDL2 Testing", 640, 480);
    Game game(window);

	bool gameRunning = true;
    SDL_Event event{};
	bool shouldDisplay = true;

    Shakkar::inputs user;
    double alpha = 0.0;
    Uint64 last_time = SDL_GetPerformanceCounter();

    while (gameRunning) {

        ProcessInputs(event, shouldDisplay, user, gameRunning);

        if (shouldDisplay) { // skip frames that cant be shown due to window not currently accepting frames to display

            const auto now = SDL_GetPerformanceCounter();

            alpha += (double)((double)(now - last_time) / SDL_GetPerformanceFrequency() * UPDATES_PER_SECOND);
            last_time = now;

            while (alpha > 1.0) {
                if (!game.update(user)) {
                    gameRunning = false;
                    break;
                }
                user.update();

                alpha -= 1.0;
            }
            if (gameRunning) {
                game.GameRender(window);
            }

        }
        else {
            const auto now = SDL_GetPerformanceCounter();
            alpha = 0;
            last_time = now;
        }
        //window.display();
    }
	return 0;
}



void ProcessInputs(SDL_Event& event, bool& shouldDisplay, Shakkar::inputs& input, bool& gameRunning) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
            case SDL_WINDOWEVENT_MINIMIZED:
                shouldDisplay = false;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                shouldDisplay = true;
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                //windowSizedChanged = true;
                break;
            default:
                break;
            }
        } break;
        case SDL_DROPFILE: {
            input.updateDroppedFile(event.drop.file);
            SDL_free(event.drop.file);
        } break;
        default: {
            if ((event.key.type == SDL_KEYDOWN) || (event.key.type == SDL_KEYUP)) {
                if (event.key.state == SDL_PRESSED) {
                    if (event.key.repeat)
                        continue;
                    input.addKey(event.key.keysym.sym);
                    // std::cout << "Physical" << SDL_GetScancodeName(event.key.keysym.scancode) << " key acting as the " << SDL_GetKeyName(event.key.keysym.sym) << " key" << std::endl;
                }
                else {
                    input.removeKey(event.key.keysym.sym);
                }
            }
            // mouse events
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                input.updateMouseButtons(event.button.button, true);
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
                input.updateMouseButtons(event.button.button, false);
            }
            else if (event.type == SDL_MOUSEMOTION) {
                input.updateMousePos(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
            }
            else if (event.type == SDL_MOUSEWHEEL) {
                input.updateMouseWheel(event.wheel.x, event.wheel.y, event.wheel.direction);
            }
            else if (event.type == SDL_QUIT)  // X button on window, or something else that closes the window from the OS
                gameRunning = false;
        } break;
        }
    }
}
