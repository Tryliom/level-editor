#include "Displays/MainMenu.h"

#include "Input.h"
#include "Window.h"
#include "AudioManager.h"
#include "DisplayManager.h"
#include "Types.h"

MainMenu::MainMenu() : _backgroundGrid(20, 21, 32) {}

void MainMenu::OnStart(Window& window)
{
    AudioManager::StopAll();
    AudioManager::Play((int) AudioType::MainMenu, true);
}


void MainMenu::Update(Window& window)
{
    if (Input::IsKeyPressed(KB_KEY_1))
    {
        window.SetDisplay(DisplayManager::GetDisplay((int) DisplayType::SnakeGame));
    }
    else if (Input::IsKeyPressed(KB_KEY_2))
    {
        window.SetDisplay(DisplayManager::GetDisplay((int) DisplayType::Editor));
    }
}

void MainMenu::Draw(Window& window)
{
	const auto height = window.Height;

    _backgroundGrid.Draw(window);

    window.DrawText({ .Text = "Press 1 to play", .Position = { window.Width / 2, height / 2 - 50}, .Pivot = Center });
    window.DrawText({ .Text = "Press 2 to edit", .Position = { window.Width / 2, height / 2 + 50}, .Pivot = Center });
}