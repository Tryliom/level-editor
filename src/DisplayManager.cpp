#include "DisplayManager.h"

#include "Displays/MainMenu.h"
#include "Displays/SnakeGame.h"
#include "Displays/Editor.h"

Display* mainMenu;
Display* snakeGame;
Display* editor;

namespace DisplayManager
{
    void Init()
    {
        mainMenu = new MainMenu();
        snakeGame = new SnakeGame();
        editor = new Editor();
    }

    Display* GetMainMenu()
    {
        return mainMenu;
    }

    Display* GetSnakeGame()
    {
        return snakeGame;
    }

    Display* GetEditor()
    {
        return editor;
    }
}