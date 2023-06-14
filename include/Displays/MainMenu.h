#pragma once

#include "Display.h"
#include "Grid.h"

class MainMenu : public Display
{
public:
    MainMenu();

private:
	Grid _backgroundGrid;

public:
    void OnStart(Window& window) override;
    void Update(Window& window) override;
    void Draw(Window& window) override;
};