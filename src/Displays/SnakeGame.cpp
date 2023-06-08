#include "Displays/SnakeGame.h"

#include "Displays/MainMenu.h"
#include "Input.h"
#include "Window.h"

const uint32_t width = 640;
const uint32_t height = 640;

SnakeGame::SnakeGame() : Display(width, height) {}

void SnakeGame::Update(Window& window)
{
    if (Input::IsKeyPressed(KB_KEY_ESCAPE))
    {
        window.SetDisplay(new MainMenu());
    }
}

void SnakeGame::Draw(Window& window)
{
    window.SetBackgroundColor(Color::Black);
}