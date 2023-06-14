#include "Displays/SnakeGame.h"

#include "Input.h"
#include "Window.h"
#include "Timer.h"
#include "AudioManager.h"
#include "DisplayManager.h"

#ifdef __EMSCRIPTEN__
#define IMAGE_PATH "assets/"
#else
#define IMAGE_PATH "../assets/"
#endif

SnakeGame::SnakeGame() :
    _grid(20, 20, 32),
    _snakeBodyImage(IMAGE_PATH "snake_part.png"),
    _snakeHeadImage(IMAGE_PATH "snake_head.png") {}

void SnakeGame::OnStart(Window& window)
{
    Reset();
}

void SnakeGame::Update(Window& window)
{
    if (Input::IsKeyPressed(KB_KEY_ESCAPE))
    {
        window.SetDisplay(DisplayManager::GetMainMenu());
    }

	if (Input::IsKeyPressed(KB_KEY_SPACE) && _waitToStart)
	{
		_waitToStart = false;

		AudioManager::StopAll();
		AudioManager::Play(AudioType::Play, true);
	}

	if (_waitToStart)
	{
		return;
	}

	if (!_gameOver)
	{
		_snakeMaxTimer = 1.f / (15.f + _score * 0.01f);
		_snakeTimer += Timer::GetSmoothDeltaTime();

		if (Input::IsKeyPressed(KB_KEY_W) && _snakeDirection != Direction::Down)
		{
			_beforeUpdateDirection = Direction::Up;
		}
		else if (Input::IsKeyPressed(KB_KEY_S) && _snakeDirection != Direction::Up)
		{
			_beforeUpdateDirection = Direction::Down;
		}
		else if (Input::IsKeyPressed(KB_KEY_A) && _snakeDirection != Direction::Right)
		{
			_beforeUpdateDirection = Direction::Left;
		}
		else if (Input::IsKeyPressed(KB_KEY_D) && _snakeDirection != Direction::Left)
		{
			_beforeUpdateDirection = Direction::Right;
		}

		if (_snakeTimer >= _snakeMaxTimer)
		{
			_snakeTimer = 0.f;
			_snakeDirection = _beforeUpdateDirection;

			UpdateSnake();
			CheckCollision(window);
		}

		if (_gameOver)
		{
			AudioManager::StopAll();
			AudioManager::Play(AudioType::GameOver, true);
		}
	}
	else
	{
		if (Input::IsKeyPressed(KB_KEY_R))
		{
			Reset();

			AudioManager::StopAll();
			AudioManager::Play(AudioType::MainMenu, true);
		}
	}
}

void SnakeGame::Draw(Window& window)
{
    window.SetBackgroundColor(Color::Black);

	window.DrawFullRectangle(0, 0, window.Width, 32, Utility::ToColor(129, 133, 255));
	window.DrawText({ .Text = std::to_string(_score) + " PTS", .Size = 22.f, .Position = { window.Width / 2, 25 }, .Pivot = Pivot::Center });

	_grid.Draw(window);

	for (int i = 0; i < _snakePositions.size(); i++)
	{
		auto position = _snakePositions[i];

		if (i == 0)
		{
			window.DrawImage(_snakeHeadImage, position.X, position.Y, Pivot::Center);
		}
		else
		{
			window.DrawImage(_snakeBodyImage, position.X, position.Y, Pivot::Center);
		}
	}

	if (_gameOver)
	{
		window.DrawFullRectangle(0, 0, window.Width, window.Height, Utility::ToColor(0, 0, 0, 100));
		window.DrawText({ .Text = "Game Over!", .Position = { window.Width / 2, window.Height / 2 - 50 }, .Color = (int) Color::Red, .Pivot = Pivot::Center });
		window.DrawText({ .Text = "Press R to restart", .Position = { window.Width / 2, window.Height / 2 + 50 }, .Color = (int) Color::White, .Pivot = Pivot::Center });
	}

	if (_waitToStart)
	{
		window.DrawFullRectangle(0, 0, window.Width, window.Height, Utility::ToColor(0, 0, 0, 100));
		window.DrawText({ .Text = "Press Space to start", .Position = { window.Width / 2, window.Height / 2 }, .Color = (int) Color::White, .Pivot = Pivot::Center });
	}
}

void SnakeGame::ChooseEggPosition()
{
	do
	{
		_grid.ReplaceAll((int) TileType::Bonus, (int) TileType::Empty);

		auto position = _eggPossiblePositions[Utility::Range(0, _eggPossiblePositions.size() - 1)];

		_grid.SetTile(position, (int) TileType::Bonus);

		auto eggPosition = _grid.FindFirst((int) TileType::Bonus, false) + Vector2I(16, 16);

		// Check if any of the snake parts are on the egg position
		bool isSnakeOnEgg = false;

		for (auto snakePosition : _snakePositions)
		{
			if (Utility::GetDistance(snakePosition, (Vector2F) eggPosition) < 16.f)
			{
				isSnakeOnEgg = true;
				break;
			}
		}

		if (!isSnakeOnEgg)
		{
			break;
		}
	}
	while (true);
}

void SnakeGame::UpdateSnake()
{
	auto lastPosition = _snakePositions[0];

	if (_snakeDirection == Direction::Up)
	{
		_snakePositions[0].Y -= 16.f;
		_snakeHeadImage.SetRotation(180);
	}
	else if (_snakeDirection == Direction::Down)
	{
		_snakePositions[0].Y += 16.f;
		_snakeHeadImage.SetRotation(0);
	}
	else if (_snakeDirection == Direction::Left)
	{
		_snakePositions[0].X -= 16.f;
		_snakeHeadImage.SetRotation(90);
	}
	else if (_snakeDirection == Direction::Right)
	{
		_snakePositions[0].X += 16.f;
		_snakeHeadImage.SetRotation(270);
	}

	// Update the snake body
	for (int i = 1; i < _snakePositions.size(); i++)
	{
		auto position = _snakePositions[i];

		_snakePositions[i] = lastPosition;

		lastPosition = position;
	}
}

void SnakeGame::CheckCollision(Window& window)
{
	// Check if the snake is out of bounds
	auto headPosition = _snakePositions[0];

	if (headPosition.X < 0 || headPosition.X >= window.Width || headPosition.Y < 0 || headPosition.Y >= window.Height)
	{
		_gameOver = true;
		return;
	}

	// Check if the snake is colliding with itself
	for (int i = 1; i < _snakePositions.size(); i++)
	{
		auto position = _snakePositions[i];

		// Check the distance between the head and the body part
		if (Utility::GetDistance(headPosition, position) < 16)
		{
			_gameOver = true;
			return;
		}
	}

	// Check if the snake is colliding with a wall
	for (auto position : _walls)
	{
		// Check the distance between the head and the wall
		if (Utility::GetDistance(headPosition, (Vector2F) position + Vector2F(16, 16)) < 16)
		{
			_gameOver = true;
			return;
		}
	}

	// Check if the snake is colliding with an egg
	auto eggPosition = _grid.FindFirst((int) TileType::Bonus, false) + Vector2I(16, 16);

	if (Utility::GetDistance(headPosition, (Vector2F) eggPosition) < 16)
	{
		// Add a new part to the snake
		_snakePositions.push_back(_snakePositions[_snakePositions.size() - 1]);
		_score += 10;
		AudioManager::Play(AudioType::Egg);

		ChooseEggPosition();
	}
}

void SnakeGame::Reset()
{
	_snakePositions.clear();

	_grid.SetYOffset(32);
	_grid.Serialize(IMAGE_PATH "levels/level.level", Load);

	_waitToStart = true;
	_gameOver = false;
	_score = 0;

	_snakeDirection = Direction::Down;
	_beforeUpdateDirection = _snakeDirection;
	_eggPossiblePositions = _grid.FindAll((int) TileType::Bonus);
	_walls = _grid.FindAll((int) TileType::Block, false);

	// Spawn the snake at the start position and 2 more parts
	auto startPosition = (Vector2F) _grid.FindFirst((int) TileType::Start) + Vector2F(8, 8);

	_snakePositions.push_back(startPosition);
	_snakePositions.push_back(startPosition + Vector2F(0, -16));
	_snakePositions.push_back(startPosition + Vector2F(0, -32));

	ChooseEggPosition();

	_grid.ReplaceAll((int) TileType::Start, (int) TileType::Empty);
}