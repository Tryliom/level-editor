#pragma once

class Display;

namespace DisplayManager
{
    void Init();

    Display* GetMainMenu();
    Display* GetSnakeGame();
    Display* GetEditor();
}