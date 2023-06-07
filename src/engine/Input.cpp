#include "engine/Input.h"

#include "MiniFB.h"
#include <cstring>

bool keys[KB_KEY_LAST + 1];
bool previousKeys[KB_KEY_LAST + 1];

bool mouseButtons[MOUSE_BTN_7 + 1];
bool previousMouseButtons[MOUSE_BTN_7 + 1];

void OnKey(struct mfb_window *, mfb_key key, mfb_key_mod, bool pressed)
{
    keys[key] = pressed;
}

void OnMouse(struct mfb_window *, mfb_mouse_button button, mfb_key_mod, bool pressed)
{
    mouseButtons[button] = pressed;
}

namespace Input
{
    void Initialize(struct mfb_window* window)
    {
        mfb_set_keyboard_callback(window, OnKey);
        mfb_set_mouse_button_callback(window, OnMouse);
    }

    void Update()
    {
        memcpy(previousKeys, keys, sizeof(keys));
        memcpy(previousMouseButtons, mouseButtons, sizeof(mouseButtons));
    }

    bool IsKeyPressed(int key)
    {
        return keys[key] && !previousKeys[key];
    }

    bool IsKeyReleased(int key)
    {
        return !keys[key] && previousKeys[key];
    }

    bool IsKeyHeld(int key)
    {
        return keys[key];
    }

    bool IsMouseButtonPressed(int button)
    {
        return mouseButtons[button] && previousMouseButtons[button] == 0;
    }

    bool IsMouseButtonReleased(int button)
    {
        return mouseButtons[button] == 0 && previousMouseButtons[button];
    }

    bool IsMouseButtonHeld(int button)
    {
        return mouseButtons[button];
    }
}