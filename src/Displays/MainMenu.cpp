#include "Displays/MainMenu.h"

#include "Input.h"
#include "Displays/SnakeGame.h"
#include "Displays/Editor.h"
#include "Window.h"

const uint32_t width = 640;
const uint32_t height = 640;

MainMenu::MainMenu() : Display(width, height) {}

void MainMenu::Update(Window& window)
{
    if (Input::IsKeyPressed(KB_KEY_1))
    {
        window.SetDisplay(new SnakeGame());
    }
    else if (Input::IsKeyPressed(KB_KEY_2))
    {
        window.SetDisplay(new Editor());
    }
}

void MainMenu::Draw(Window& window)
{
    window.SetBackgroundColor(Color::Black);
    window.DrawText({ .Text = "Press 1 to play Snake", .Position = { window.Width / 2, 30}, .Pivot = Center });
    window.DrawText({ .Text = "Press 2 to edit Snake", .Position = { window.Width / 2, 100}, .Pivot = Center });
}