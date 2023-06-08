#pragma once

#include "Display.h"

class MainMenu : public Display
{
public:
    MainMenu();

    void Update(Window& window) override;
    void Draw(Window& window) override;
};