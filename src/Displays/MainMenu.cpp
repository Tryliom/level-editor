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

	ClearButtons();

	AddButton(
		Button("Play", Vector2F{ static_cast<float>(window.Width / 2), static_cast<float>(window.Height / 2 - 50) }, Vector2I{ 200, 50 }),
		[](Window& window) {
			window.SetDisplay(DisplayManager::GetDisplay((int) DisplayType::SnakeGame));
		}
	);
	AddButton(
		Button("Edit", Vector2F{ static_cast<float>(window.Width / 2), static_cast<float>(window.Height / 2 + 50) }, Vector2I{ 200, 50 }),
		[](Window& window) {
			window.SetDisplay(DisplayManager::GetDisplay((int) DisplayType::Editor));
		}
	);
}

void MainMenu::Draw(Window& window)
{
    _backgroundGrid.Draw(window);
}