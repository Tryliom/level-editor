#include "Window.h"
#include "DisplayManager.h"
#include "AudioManager.h"
#include "Types.h"
#include "Displays/MainMenu.h"
#include "Displays/SnakeGame.h"
#include "Displays/Editor.h"
#include "FrameAllocator.h"
#include "Logger.h"

#ifdef __EMSCRIPTEN__
#define AUDIO_PATH "assets/audio/"
#else
#define AUDIO_PATH "../assets/audio/"
#endif


int main()
{
	// Pre-definitions
	DisplayManager::AddDisplayType((int) DisplayType::MainMenu, new MainMenu());
	DisplayManager::AddDisplayType((int) DisplayType::SnakeGame, new SnakeGame());
	DisplayManager::AddDisplayType((int) DisplayType::Editor, new Editor());

	AudioManager::Init();

	AudioManager::AddSample((int) AudioType::MainMenu, AUDIO_PATH "mainMenu.ogg");
	AudioManager::AddSample((int) AudioType::Play, AUDIO_PATH "play.ogg");
	AudioManager::AddSample((int) AudioType::GameOver, AUDIO_PATH "gameOver.ogg");
	AudioManager::AddSample((int) AudioType::Egg, AUDIO_PATH "egg.ogg");

    const uint32_t width = 640;
    const uint32_t height = 672;

    FrameAllocator::Initialize(1024 * 1024 * 10);

    Window window(width, height);

    window.SetDisplay(DisplayManager::GetDisplay((int) DisplayType::MainMenu));

    do {
        window.Update();

        LOG(FrameAllocator::Format("This log is useless but I can show you at least that godly number: %i", 666));

        FrameAllocator::OnFrameEnd();
    }
    while (window.IsOpen());
}