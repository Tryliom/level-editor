#include "Grid.h"

#if defined(_WIN32)
#include "malloc.h"
#endif

std::vector<Image> Grid::ToImage =
{
    Image("../assets/empty.png"),
    Image("../assets/grass.png"),
    Image("../assets/dirt.png"),
    Image("../assets/block.png")
};

Grid::Grid(uint32_t width, uint32_t height, uint32_t tileSize, int defaultHighlightColor)
{
    this->width = width;
    this->height = height;
    this->tileSize = tileSize;

    this->xOffset = 0;
    this->yOffset = 0;

    grid = (int*) malloc(width * height * sizeof(int));
    highlightGrid = (bool*) malloc(width * height * sizeof(bool));

    hasBeenHighlighted = false;
    this->defaultHighlightColor = defaultHighlightColor;

    Clear();
    ClearHighlighted();
}

uint32_t Grid::ToGridPosition(Vector2I position, bool local) const
{
    if (!local)
    {
        position.X -= xOffset;
        position.Y -= yOffset;
    }

    if (position.X < 0 || position.Y < 0)
    {
        return 0;
    }

    return (uint32_t) (position.X / tileSize) + (uint32_t) (position.Y / tileSize) * width;
}

void Grid::Draw(Window& window)
{
    for (uint32_t i = 0; i < width * height; i++)
    {
        window.DrawImage(ToImage[grid[i]], xOffset + (i % width * tileSize), yOffset + (i / width * tileSize), Pivot::TopLeft);
    }

    if (hasBeenHighlighted)
    {
        for (uint32_t i = 0; i < width * height; i++)
        {
            if (highlightGrid[i])
            {
                if (useHighlightImage)
                {
                    window.DrawImage(highlightImage, xOffset + (i % width * tileSize), yOffset + (i / width * tileSize), Pivot::TopLeft);
                    continue;
                }

                window.DrawRectangle(xOffset + (i % width * tileSize), yOffset + (i / width * tileSize), tileSize, tileSize, defaultHighlightColor);
            }
        }
    }

    hasBeenHighlighted = false;

    ClearHighlighted();
}

void Grid::Clear()
{
    for (uint32_t i = 0; i < width * height; i++)
    {
        grid[i] = 0;
    }
}

void Grid::ClearHighlighted()
{
    for (uint32_t i = 0; i < width * height; i++)
    {
        highlightGrid[i] = false;
    }
}

void Grid::HighlightTile(Vector2I position, bool local)
{
    const uint32_t gridPosition = ToGridPosition(position, local);

    if (!local)
    {
        position.X -= xOffset;
        position.Y -= yOffset;
    }

    if (gridPosition >= width * height || position.X / tileSize >= width || position.Y / tileSize >= height)
    {
        return;
    }

    highlightGrid[gridPosition] = true;
    hasBeenHighlighted = true;
    useHighlightImage = false;
}

void Grid::HighlightTile(Vector2I position, Image image, bool local)
{
    const uint32_t gridPosition = ToGridPosition(position, local);

    if (!local)
    {
        position.X -= xOffset;
        position.Y -= yOffset;
    }

    if (gridPosition >= width * height || position.X / tileSize >= width || position.Y / tileSize >= height)
    {
        return;
    }

    highlightGrid[gridPosition] = true;
    hasBeenHighlighted = true;
    useHighlightImage = true;
    highlightImage = image;
}

void Grid::DrawTile(Vector2I position, int tileType, bool local)
{
    const uint32_t gridPosition = ToGridPosition(position, local);

    if (!local)
    {
        position.X -= xOffset;
        position.Y -= yOffset;
    }

    if (grid[gridPosition] == tileType || gridPosition >= width * height
        || position.X / tileSize >= width || position.Y / tileSize >= height)
    {
        return;
    }

    grid[gridPosition] = tileType;
}

Grid::~Grid()
{
    free(grid);
    free(highlightGrid);
}

int Grid::GetTile(Vector2I position, bool local) const
{
    const uint32_t gridPosition = ToGridPosition(position, local);

    if (!local)
    {
        position.X -= xOffset;
        position.Y -= yOffset;
    }

    if (gridPosition >= width * height || position.X / tileSize >= width || position.Y / tileSize >= height)
    {
        return ToImage.size();
    }

    return grid[gridPosition];
}

bool Grid::IsOnTile(Vector2I position, bool local) const
{
    return GetTile(position, local) != ToImage.size();
}

void Grid::SetXOffset(int value)
{
    this->xOffset = value;
}

void Grid::SetYOffset(int value)
{
    this->yOffset = value;
}