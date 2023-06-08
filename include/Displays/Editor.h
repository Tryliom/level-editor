#pragma once

#include "Display.h"

class Editor : public Display
{
public:
    Editor();

    void Update(Window& window) override;
    void Draw(Window& window) override;
};