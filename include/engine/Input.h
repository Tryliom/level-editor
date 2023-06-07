#pragma once

#include "MiniFB.h"
#include "Utility.h"

namespace Input
{
    void Initialize(struct mfb_window* window);

    void Update();

    bool IsKeyPressed(int key);
    bool IsKeyReleased(int key);
    bool IsKeyHeld(int key);

    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonReleased(int button);
    bool IsMouseButtonHeld(int button);
}