
#include "inputs.hpp"
#include <algorithm>

namespace Shakkar {

    void inputs::updateMousePos(int32_t x, int32_t y, int32_t dx, int32_t dy) {
        mouse.x = x;
        mouse.y = y;
        mouse.dx = dx;
        mouse.dy = dy;
    }

    void inputs::updateMouseWheel(int32_t wx, int32_t wy, int32_t dwx) {
        mouse.wx = wx;
        mouse.wy = wy;

        if (dwx == SDL_MOUSEWHEEL_FLIPPED) {
            mouse.wx = -wx;
            mouse.wy = -wy;
        }
    }

    void inputs::updateMouseButtons(uint32_t which, bool state) {
        switch (which) {
        case SDL_BUTTON_LEFT:
            mouse.left_held = state;
            mouse.left_pressed = state;
            break;
        case SDL_BUTTON_RIGHT:
            mouse.right_held = state;
            mouse.right_pressed = state;
            break;
        case SDL_BUTTON_MIDDLE:
            mouse.middle_held = state;
            mouse.middle_pressed = state;
            break;
        default:
            break;
        }
    }

    void inputs::updateDroppedFile(const char* file) {
        dropped_file = file;
    }

    void inputs::addKey(SDL_Keycode key) {
        cur_buttons.insert(key);
    }

    void inputs::removeKey(SDL_Keycode key) {
        cur_buttons.erase(key);
    }

    Mouse inputs::getMouse() const {
        return mouse;
    }

    Key inputs::getKey(SDL_Keycode key) const {
        Key keyState{};

        auto cur = std::find(cur_buttons.begin(), cur_buttons.end(), key) != cur_buttons.end();

        auto prev = std::find(prev_buttons.begin(), prev_buttons.end(), key) != prev_buttons.end();

        keyState.held = cur && prev;
        keyState.pressed = justPressed(prev, cur);
        keyState.released = !cur && prev;

        return keyState;
    }

    std::string inputs::getDroppedFile() const {
        return dropped_file;
    }

    void inputs::update() {
        prev_buttons = cur_buttons;

        mouse.dx = 0;
        mouse.dy = 0;

        mouse.wx = 0;
        mouse.wy = 0;

        mouse.left_pressed = false;
        mouse.right_pressed = false;
        mouse.middle_pressed = false;

        dropped_file = "";
    }
};
