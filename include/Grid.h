#pragma once

#include "engine/Vector.h"
#include "engine/Window.h"

enum TileType
{
    Empty = 0,
    Grass,
    Dirt,
    Block,
    End
};

class Grid
{
public:
    Grid(uint32_t width, uint32_t height, uint32_t tileSize, int defaultHighlightColor = Utility::ToColor(0, 200, 100));
    ~Grid();

private:
    uint32_t width;
    uint32_t height;
    uint32_t tileSize;
    int xOffset;
    int yOffset;

    TileType* grid;
    bool* highlightGrid;

    bool hasBeenHighlighted;
    int defaultHighlightColor;
    Image highlightImage;
    bool useHighlightImage;

    [[nodiscard]] uint32_t ToGridPosition(Vector2I position, bool local = true) const;

public:
    static Image TileTypeToImage[TileType::End];

    void Draw(Window& window);

    void Clear();
    void ClearHighlighted();

    void HighlightTile(Vector2I position, bool local = true);
    void HighlightTile(Vector2I position, Image image, bool local = true);
    void DrawTile(Vector2I position, TileType tileType, bool local = true);

    [[nodiscard]] TileType GetTile(Vector2I position, bool local = true) const;
    [[nodiscard]] bool IsOnTile(Vector2I position, bool local = true) const;
    void SetXOffset(int value);
    void SetYOffset(int value);
};