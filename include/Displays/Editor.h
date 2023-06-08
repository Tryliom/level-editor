#pragma once

#include "Display.h"
#include "Grid.h"

class Editor : public Display
{
public:
    Editor();

private:
	Grid _grid;
	Grid _tileSelector;

public:

    void Update(Window& window) override;
    void Draw(Window& window) override;
};