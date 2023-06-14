#include "Window.h"
#include "DisplayManager.h"

int main()
{
    const uint32_t width = 640;
    const uint32_t height = 672;

    DisplayManager::Init();
    Window window(width, height);

    window.SetDisplay(DisplayManager::GetMainMenu());

    do {
        window.Update();
    }
    while (window.IsOpen());
}