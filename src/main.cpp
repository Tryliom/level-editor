#include "engine/Window.h"
#include "engine/Utility.h"
#include "engine/Input.h"

#include "Grid.h"

Image SelectedImages[TileType::End]
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
auto selectedTile = Empty;
Image selectedTileImage;

Window window(width, height);
Grid grid(gridWidth, gridHeight, tileSize);
Grid tileSelector(TileType::End, 1, tileSize, Utility::ToColor(255, 100, 100));

int main()
{
    for (auto & SelectedImage : SelectedImages)
    {
        SelectedImage.SetColor(Utility::ToColor(255, 255, 255, 150));
    }

    selectedTileImage = SelectedImages[selectedTile];
    tileSelector.SetXOffset(gridWidth * tileSize);

    for (int i = 0; i < (int) TileType::End; i++)
    {
        tileSelector.DrawTile(Vector2I(i * tileSize, 0), (TileType) i);
    }

    do {
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
                grid.DrawTile(mousePosition, Empty);
            }
        }

        tileSelector.HighlightTile(Vector2I(selectedTile * tileSize, 0), true);

        window.SetBackgroundColor(Color::Black);
        grid.Draw(window);
        tileSelector.Draw(window);

        window.Update();
    }
    while (window.IsOpen());
}