#include "Window.h"
#include "Displays/MainMenu.h"

int main()
{
    const uint32_t width = 1000;
    const uint32_t height = 640;

    Window window(width, height);

    window.SetDisplay(new MainMenu());

    do {
        window.Update();
    }
    while (window.IsOpen());
}