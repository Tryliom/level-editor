#pragma once

#include "Display.h"

class SnakeGame : public Display
{
public:
    SnakeGame();

    void Update(Window& window) override;
    void Draw(Window& window) override;
};