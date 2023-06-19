#pragma once

#include <vector>
#include "Vector.h"
#include "Window.h"

enum SerializeMode
{
    Save,
    Load
};

enum class TileType
{
	Empty,
	Block,
	Bonus,
	Start
};

class Grid
{
public:
    Grid(uint32_t width, uint32_t height, uint32_t tileSize, int defaultHighlightColor = Utility::ToColor(0, 200, 100));

private:
    uint32_t _width;
    uint32_t _height;
    uint32_t _tileSize;
    int _xOffset;
    int _yOffset;

    int* _grid;
    bool* _highlightGrid;

    bool _hasBeenHighlighted;
    int _defaultHighlightColor;
    Image _highlightImage;
    bool _useHighlightImage { false };

    bool _saveHistory { false };
    int _maxHistory { 20000 };
    int* _history[20000]{};
    int _historyIndex { 0 };

    [[nodiscard]] uint32_t ToGridPosition(Vector2I position, bool local = true) const;

public:
    static std::vector<Image> ToImage;

    void Draw(Window& window);

    void Clear();
    void ClearHighlighted();

    void HighlightTile(Vector2I position, bool local = true);
    void HighlightTile(Vector2I position, Image image, bool local = true);
    void SetTile(Vector2I position, int tileType, bool local = true);

    [[nodiscard]] int GetTile(Vector2I position, bool local = true) const;
    [[nodiscard]] bool IsOnTile(Vector2I position, bool local = true) const;

    void SetXOffset(int value);
    void SetYOffset(int value);

    void EnableHistory();
    void Undo();
    void AddToHistory();

    void Serialize(const std::string& path, SerializeMode mode);

	void ReplaceAll(int tileType, int newTileType);
	std::vector<Vector2I> FindAll(int tileType, bool local = true);
	Vector2I FindFirst(int tileType, bool local = true);
};