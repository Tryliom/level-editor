#include "Displays/Editor.h"

#include <vector>

#include "Grid.h"
#include "Image.h"
#include "Utility.h"
#include "Input.h"
#include "Window.h"
#include "DisplayManager.h"
#include "Types.h"

#ifdef __EMSCRIPTEN__
#define IMAGE_PATH "assets/"
#else
#define IMAGE_PATH "../assets/"
#endif


std::vector<Image> SelectedImages =
{
    Image(IMAGE_PATH "empty.png"),
	Image(IMAGE_PATH "block.png"),
	Image(IMAGE_PATH "bonus.png"),
	Image(IMAGE_PATH "start.png"),
};

const uint32_t width = 1000;
const uint32_t height = 640;
const uint32_t gridWidth = 20;
const uint32_t gridHeight = 20;
const uint32_t tileSize = 32;
int selectedTile;
Image selectedTileImage;

Editor::Editor() :
    _grid(gridWidth, gridHeight, tileSize),
    _tileSelector(Grid::ToImage.size(), 1, tileSize, Utility::ToColor(255, 100, 100)) {}

void Editor::OnStart(Window& window)
{
    _grid.EnableHistory();
    _grid.AddToHistory();

    for (auto & SelectedImage : SelectedImages)
    {
        SelectedImage.SetColor(Utility::ToColor(255, 255, 255, 150));
    }

    selectedTileImage = SelectedImages[selectedTile];
    _grid.SetYOffset(tileSize);

    for (int i = 0; i < Grid::ToImage.size(); i++)
    {
        _tileSelector.SetTile(Vector2I(i * tileSize, 0), i);
    }
}

void Editor::Update(Window& window)
{
    auto mousePosition = window.GetMousePosition();

    if (_tileSelector.IsOnTile(mousePosition, false))
    {
        _grid.ClearHighlighted();
        _tileSelector.HighlightTile(mousePosition, false);

        if (Input::IsMouseButtonPressed(MOUSE_LEFT))
        {
            selectedTile = _tileSelector.GetTile(mousePosition, false);
            selectedTileImage = SelectedImages[selectedTile];
        }
    }
    else
    {
        _tileSelector.ClearHighlighted();
        _grid.HighlightTile(mousePosition, selectedTileImage, false);

        if (Input::IsMouseButtonHeld(MOUSE_LEFT))
        {
	        _grid.SetTile(mousePosition, selectedTile, false);
        }
        else if (Input::IsMouseButtonHeld(MOUSE_RIGHT))
        {
	        _grid.SetTile(mousePosition, 0, false);
        }
    }

    if (Input::IsKeyHeld(KB_KEY_RIGHT_CONTROL) || Input::IsKeyHeld(KB_KEY_LEFT_CONTROL))
    {
        if (Input::IsKeyPressed(KB_KEY_S))
        {
            _grid.Serialize(IMAGE_PATH "levels/level.level", Save);
        }
        else if (Input::IsKeyPressed(KB_KEY_L))
        {
            _grid.Serialize(IMAGE_PATH "levels/level.level", Load);
        }
        else if (Input::IsKeyPressed(KB_KEY_Y))
        {
            _grid.Undo();
        }
    }

    _tileSelector.HighlightTile(Vector2I(selectedTile * tileSize, 0), true);

    if (Input::IsKeyPressed(KB_KEY_ESCAPE))
    {
        window.SetDisplay(DisplayManager::GetDisplay((int) DisplayType::MainMenu));
    }
}

void Editor::Draw(Window& window)
{
    window.SetBackgroundColor(Color::Black);
    _grid.Draw(window);
    _tileSelector.Draw(window);
}
