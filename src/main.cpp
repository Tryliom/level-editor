#include "Window.h"
#include "DisplayManager.h"
#include "FrameAllocator.h"

int main()
{
    const uint32_t width = 640;
    const uint32_t height = 672;

    DisplayManager::Init();
    FrameAllocator::Initialize(1000);
    Window window(width, height);

    window.SetDisplay(DisplayManager::GetMainMenu());

    do {
        window.Update();
    }
    while (window.IsOpen());
}