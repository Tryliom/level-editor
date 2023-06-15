#include "Displays/MainMenu.h"

#include "Input.h"
#include "Window.h"
#include "AudioManager.h"
#include "DisplayManager.h"
#include "FrameAllocator.h"

MainMenu::MainMenu() : _backgroundGrid(20, 21, 32) {}

void MainMenu::OnStart(Window& window)
{
    AudioManager::StopAll();
    AudioManager::Play(AudioType::MainMenu, true);
}


void MainMenu::Update(Window& window)
{
    if (Input::IsKeyPressed(KB_KEY_1))
    {
        window.SetDisplay(DisplayManager::GetSnakeGame());
    }
    else if (Input::IsKeyPressed(KB_KEY_2))
    {
        window.SetDisplay(DisplayManager::GetEditor());
    }
}

void MainMenu::Draw(Window& window)
{
	const auto height = window.Height;

    _backgroundGrid.Draw(window);

    window.DrawText({ .Text = "Press 1 to play", .Position = { window.Width / 2, height / 2 - 50}, .Pivot = Center });
    window.DrawText({ .Text = "Press 2 to edit", .Position = { window.Width / 2, height / 2 + 50}, .Pivot = Center });

    int y = height / 2 - 100;

    for (int i = 0; i < 5; i++)
    {
        const char* text = (char*) FrameAllocator::Allocate(15);
        snprintf((char*) text, 15, "test: %i", window.Frame);

        window.DrawText({ .Text = text, .Position = { window.Width / 2 - 100, y}, .Pivot = TopLeft });

        y -= 40;
    }

    FrameAllocator::OnFrameEnd();
}