#pragma once

#include "Display.h"
#include "Grid.h"

enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

class SnakeGame : public Display
{
	enum class ButtonType
	{
		Start, Restart
	};

public:
	SnakeGame();

private:
	Grid _grid;
	bool _waitToStart{};
	std::vector<Vector2F> _snakePositions;
	Direction _snakeDirection { Direction::Down };
	Direction _beforeUpdateDirection { Direction::Down };
	float _snakeTimer { 0.f };
	float _snakeMaxTimer { 1.f / 7.f };
	std::vector<Vector2I> _eggPossiblePositions;
	std::vector<Vector2I> _walls;

	bool _gameOver { false };
	int _score { 0 };

	Image _snakeHeadImage;
	Image _snakeBodyImage;

	void ChooseEggPosition();
	void UpdateSnake();
	void CheckCollision(Window& window);
	void Reset();
public:
    void OnStart(Window& window) override;
    void Update(Window& window) override;
    void Draw(Window& window) override;
};