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

Grid grid(gridWidth, gridHeight, tileSize);
Grid tileSelector(Grid::ToImage.size(), 1, tileSize, Utility::ToColor(255, 100, 100));

Editor::Editor() : Display(width, height)
{
    grid.EnableHistory();
    grid.AddToHistory();

    for (auto & SelectedImage : SelectedImages)
    {
        SelectedImage.SetColor(Utility::ToColor(255, 255, 255, 150));
    }

    selectedTileImage = SelectedImages[selectedTile];
    tileSelector.SetXOffset(gridWidth * tileSize);

    for (int i = 0; i < Grid::ToImage.size(); i++)
    {
        tileSelector.DrawTile(Vector2I(i * tileSize, 0), i);
    }
}

void Editor::Update(Window& window)
{
    auto mousePosition = window.GetMousePosition();

    if (tileSelector.IsOnTile(mousePosition, false))
    {
        grid.ClearHighlighted();
        tileSelector.HighlightTile(mousePosition, false);

        if (Input::IsMouseButtonPressed(MOUSE_LEFT))
        {
            selectedTile = tileSelector.GetTile(mousePosition, false);
            selectedTileImage = SelectedImages[selectedTile];
        }
    }
    else
    {
        tileSelector.ClearHighlighted();
        grid.HighlightTile(mousePosition, selectedTileImage);

        if (Input::IsMouseButtonHeld(MOUSE_LEFT))
        {
            grid.DrawTile(mousePosition, selectedTile);
        }
        else if (Input::IsMouseButtonHeld(MOUSE_RIGHT))
        {
            grid.DrawTile(mousePosition, 0);
        }
    }

    if (Input::IsKeyHeld(KB_KEY_RIGHT_CONTROL) || Input::IsKeyHeld(KB_KEY_LEFT_CONTROL))
    {
        if (Input::IsKeyPressed(KB_KEY_S))
        {
            grid.Serialize("../assets/levels/level.level", Save);
        }
        else if (Input::IsKeyPressed(KB_KEY_L))
        {
            grid.Serialize("../assets/levels/level.level", Load);
        }
        else if (Input::IsKeyPressed(KB_KEY_Y))
        {
            grid.Undo();
        }
    }

    tileSelector.HighlightTile(Vector2I(selectedTile * tileSize, 0), true);

    if (Input::IsKeyPressed(KB_KEY_ESCAPE))
    {
        window.SetDisplay(new MainMenu());
    }
}

void Editor::Draw(Window& window)
{
    window.SetBackgroundColor(Color::Black);
    grid.Draw(window);
    tileSelector.Draw(window);
}