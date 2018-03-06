#pragma once
#include <array>
#include <SDL2/SDL.h>
class KeyboardHandler {
public:
    void
    handleKeyboardEvent(SDL_KeyboardEvent event) {
        m_KeyState[event.keysym.scancode] = event.state;
    }

    bool
    isPressed(SDL_Keycode keyCode) {
        return (m_KeyState[keyCode] == SDL_PRESSED);
    }

    bool
    isReleased(SDL_Keycode keyCode) {
        return (m_KeyState[keyCode] == SDL_RELEASED);
    }
private:
    std::array<Uint8, SDL_NUM_SCANCODES> m_KeyState;
};
