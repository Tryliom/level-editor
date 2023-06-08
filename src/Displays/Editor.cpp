#include "Displays/Editor.h"

#include <vector>

#include "Grid.h"
#include "Image.h"
#include "Utility.h"
#include "Input.h"
#include "Displays/MainMenu.h"
#include "Window.h"

std::vector<Image> SelectedImages =
{
    Image("../assets/empty.png"),
    Image("../assets/grass.png"),
    Image("../assets/dirt.png"),
    Image("../assets/block.png")
};

const uint32_t width = 1000;
const uint32_t height = 640;
const uint32_t gridWidth = 20;
const uint32_t gridHeight = 20;
const uint32_t tileSize = 32;
int selectedTile;
Image selectedTileImage;

Editor::Editor() : Display(width, height), _grid(gridWidth, gridHeight, tileSize), _tileSelector(Grid::ToImage.size(), 1, tileSize, Utility::ToColor(255, 100, 100))
{
    _grid.EnableHistory();
    _grid.AddToHistory();

    for (auto & SelectedImage : SelectedImages)
    {
        SelectedImage.SetColor(Utility::ToColor(255, 255, 255, 150));
    }

    selectedTileImage = SelectedImages[selectedTile];
    _tileSelector.SetXOffset(gridWidth * tileSize);

    for (int i = 0; i < Grid::ToImage.size(); i++)
    {
        _tileSelector.DrawTile(Vector2I(i * tileSize, 0), i);
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
        _grid.HighlightTile(mousePosition, selectedTileImage);

        if (Input::IsMouseButtonHeld(MOUSE_LEFT))
        {
            _grid.DrawTile(mousePosition, selectedTile);
        }
        else if (Input::IsMouseButtonHeld(MOUSE_RIGHT))
        {
            _grid.DrawTile(mousePosition, 0);
        }
    }

    if (Input::IsKeyHeld(KB_KEY_RIGHT_CONTROL) || Input::IsKeyHeld(KB_KEY_LEFT_CONTROL))
    {
        if (Input::IsKeyPressed(KB_KEY_S))
        {
            _grid.Serialize("../assets/levels/level.level", Save);
        }
        else if (Input::IsKeyPressed(KB_KEY_L))
        {
            _grid.Serialize("../assets/levels/level.level", Load);
        }
        else if (Input::IsKeyPressed(KB_KEY_Y))
        {
            _grid.Undo();
        }
    }

    _tileSelector.HighlightTile(Vector2I(selectedTile * tileSize, 0), true);

    if (Input::IsKeyPressed(KB_KEY_ESCAPE))
    {
        window.SetDisplay(new MainMenu());
    }
}

void Editor::Draw(Window& window)
{
    window.SetBackgroundColor(Color::Black);
    _grid.Draw(window);
    _tileSelector.Draw(window);
}