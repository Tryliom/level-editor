#include "Grid.h"

#if defined(_WIN32)
#include "malloc.h"
#include "Logger.h"
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
    this->_width = width;
    this->_height = height;
    this->_tileSize = tileSize;

    this->_xOffset = 0;
    this->_yOffset = 0;

    _grid = (int*) malloc(width * height * sizeof(int));
    _highlightGrid = (bool*) malloc(width * height * sizeof(bool));

    _hasBeenHighlighted = false;
    this->_defaultHighlightColor = defaultHighlightColor;

    _historyIndex = 0;

    Clear();
    ClearHighlighted();
}

uint32_t Grid::ToGridPosition(Vector2I position, bool local) const
{
    if (!local)
    {
        position.X -= _xOffset;
        position.Y -= _yOffset;
    }

    if (position.X < 0 || position.Y < 0)
    {
        return 0;
    }

    return (uint32_t) (position.X / _tileSize) + (uint32_t) (position.Y / _tileSize) * _width;
}

void Grid::AddToHistory()
{
    if (_historyIndex == 99)
    {
        for (uint32_t i = 0; i < 99; i++)
        {
            _history[i] = _history[i + 1];
        }
    }

    // Make a copy of the grid
    _history[_historyIndex] = (int*) malloc(_width * _height * sizeof(int));
    memcpy(_history[_historyIndex], _grid, _width * _height * sizeof(int));

    if (_historyIndex < 99)
    {
        _historyIndex++;
    }
}

void Grid::Draw(Window& window)
{
    for (uint32_t i = 0; i < _width * _height; i++)
    {
        window.DrawImage(ToImage[_grid[i]], _xOffset + (i % _width * _tileSize), _yOffset + (i / _width * _tileSize), Pivot::TopLeft);
    }

    if (_hasBeenHighlighted)
    {
        for (uint32_t i = 0; i < _width * _height; i++)
        {
            if (_highlightGrid[i])
            {
                if (_useHighlightImage)
                {
                    window.DrawImage(_highlightImage, _xOffset + (i % _width * _tileSize), _yOffset + (i / _width * _tileSize), Pivot::TopLeft);
                    continue;
                }

                window.DrawRectangle(_xOffset + (i % _width * _tileSize), _yOffset + (i / _width * _tileSize), _tileSize, _tileSize, _defaultHighlightColor);
            }
        }
    }

    _hasBeenHighlighted = false;

    ClearHighlighted();
}

void Grid::Clear()
{
    for (uint32_t i = 0; i < _width * _height; i++)
    {
        _grid[i] = 0;
    }
}

void Grid::ClearHighlighted()
{
    for (uint32_t i = 0; i < _width * _height; i++)
    {
        _highlightGrid[i] = false;
    }
}

void Grid::HighlightTile(Vector2I position, bool local)
{
    const uint32_t gridPosition = ToGridPosition(position, local);

    if (!local)
    {
        position.X -= _xOffset;
        position.Y -= _yOffset;
    }

    if (gridPosition >= _width * _height || position.X / _tileSize >= _width || position.Y / _tileSize >= _height)
    {
        return;
    }

    _highlightGrid[gridPosition] = true;
    _hasBeenHighlighted = true;
    _useHighlightImage = false;
}

void Grid::HighlightTile(Vector2I position, Image image, bool local)
{
    const uint32_t gridPosition = ToGridPosition(position, local);

    if (!local)
    {
        position.X -= _xOffset;
        position.Y -= _yOffset;
    }

    if (gridPosition >= _width * _height || position.X / _tileSize >= _width || position.Y / _tileSize >= _height)
    {
        return;
    }

    _highlightGrid[gridPosition] = true;
    _hasBeenHighlighted = true;
    _useHighlightImage = true;
    _highlightImage = image;
}

void Grid::DrawTile(Vector2I position, int tileType, bool local)
{
    const uint32_t gridPosition = ToGridPosition(position, local);

    if (!local)
    {
        position.X -= _xOffset;
        position.Y -= _yOffset;
    }

    if (_grid[gridPosition] == tileType || gridPosition >= _width * _height
        || position.X / _tileSize >= _width || position.Y / _tileSize >= _height)
    {
        return;
    }

    if (_saveHistory)
    {
        AddToHistory();
    }

    _grid[gridPosition] = tileType;
}

Grid::~Grid()
{
    free(_grid);
    free(_highlightGrid);
}

int Grid::GetTile(Vector2I position, bool local) const
{
    const uint32_t gridPosition = ToGridPosition(position, local);

    if (!local)
    {
        position.X -= _xOffset;
        position.Y -= _yOffset;
    }

    if (gridPosition >= _width * _height || position.X / _tileSize >= _width || position.Y / _tileSize >= _height)
    {
        return ToImage.size();
    }

    return _grid[gridPosition];
}

bool Grid::IsOnTile(Vector2I position, bool local) const
{
    return GetTile(position, local) != ToImage.size();
}

void Grid::SetXOffset(int value)
{
    this->_xOffset = value;
}

void Grid::SetYOffset(int value)
{
    this->_yOffset = value;
}

void serializeBytes(FILE* file, SerializeMode mode, void* ptr, size_t size)
{
    if (mode == SerializeMode::Save)
    {
        fwrite(ptr, size, 1, file);
    }
    else
    {
        fread(ptr, size, 1, file);
    }
}

void Grid::Serialize(const std::string& path, SerializeMode mode)
{
    FILE* file = fopen(path.c_str(), mode == SerializeMode::Save ? "wb" : "rb");

    serializeBytes(file, mode, &_width, sizeof(uint32_t));
    serializeBytes(file, mode, &_height, sizeof(uint32_t));

    if (mode == SerializeMode::Load)
    {
        _grid = (int*) malloc(_width * _height * sizeof(int));
        _highlightGrid = (bool*) malloc(_width * _height * sizeof(bool));

        _hasBeenHighlighted = false;
    }

    serializeBytes(file, mode, _grid, sizeof(int) * _width * _height);

    fclose(file);
}

void Grid::EnableHistory()
{
    _saveHistory = true;
}

void Grid::Undo()
{
    if (_historyIndex == 0 || !_saveHistory)
    {
        return;
    }

    _historyIndex--;

    memcpy(_grid, _history[_historyIndex], _width * _height * sizeof(int));
}