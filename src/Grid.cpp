#include "Grid.h"

#if defined(_WIN32)
#include "malloc.h"
#endif

#ifdef __EMSCRIPTEN__
#define IMAGE_PATH "assets/"
#else
#define IMAGE_PATH "../assets/"
#endif

std::vector<Image> Grid::ToImage;

Grid::Grid(uint32_t width, uint32_t height, uint32_t tileSize, int defaultHighlightColor)
{
    Grid::ToImage =
    {
        Image(IMAGE_PATH "empty.png"),
        Image(IMAGE_PATH "block.png"),
        Image(IMAGE_PATH "bonus.png"),
        Image(IMAGE_PATH "start.png"),
    };

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

void Grid::SetTile(Vector2I position, int tileType, bool local)
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

    AddToHistory();

    _grid[gridPosition] = tileType;
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

void Grid::AddToHistory()
{
    if (!_saveHistory) return;

    // Check if the last history is the same as the current grid
    if (_historyIndex > 0)
    {
        bool isSame = true;

        for (uint32_t i = 0; i < _width * _height; i++)
        {
            if (_history[_historyIndex - 1][i] != _grid[i])
            {
                isSame = false;
                break;
            }
        }

        if (isSame) return;
    }

    if (_historyIndex == _maxHistory - 1)
    {
        for (uint32_t i = 0; i < _maxHistory - 1; i++)
        {
            _history[i] = _history[i + 1];
        }
    }

    // Make a copy of the grid
    _history[_historyIndex] = (int*) malloc(_width * _height * sizeof(int));
    memcpy(_history[_historyIndex], _grid, _width * _height * sizeof(int));

    if (_historyIndex < _maxHistory - 1)
    {
        _historyIndex++;
    }
}

void Grid::ReplaceAll(int tileType, int newTileType)
{
	AddToHistory();

	for (int i = 0; i < _width * _height; i++)
	{
		if (_grid[i] == tileType)
		{
			_grid[i] = newTileType;
		}
	}
}

std::vector<Vector2I> Grid::FindAll(int tileType, bool local)
{
	std::vector<Vector2I> positions;

	for (int i = 0; i < _width * _height; i++)
	{
		if (_grid[i] == tileType)
		{
			auto position = Vector2I(i % _width * _tileSize, i / _width * _tileSize);

			if (!local)
			{
				position.X += _xOffset;
				position.Y += _yOffset;
			}

			positions.push_back(position);
		}
	}

	return positions;
}

Vector2I Grid::FindFirst(int tileType, bool local)
{
	for (int i = 0; i < _width * _height; i++)
	{
		if (_grid[i] == tileType)
		{
			auto position = Vector2I(i % _width * _tileSize, i / _width * _tileSize);

			if (!local)
			{
				position.X += _xOffset;
				position.Y += _yOffset;
			}

			return position;
		}
	}

	return Vector2I(-1, -1);
}