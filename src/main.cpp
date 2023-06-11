#include "Window.h"
#include "Displays/MainMenu.h"

int main()
{
    const uint32_t width = 640;
    const uint32_t height = 672;

    Window window(width, height);

    window.SetDisplay(new MainMenu());

    do {
        window.Update();
    }
    while (window.IsOpen());
}