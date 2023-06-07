#include "engine/Window.h"

#include "MiniFB.h"
#include "engine/Input.h"
#include "engine/Timer.h"
#include "engine/Logger.h"
#include "engine/AudioManager.h"

#if defined(_WIN32)
    #include "malloc.h"
#endif

#include <cstring>
#include <cmath>
#include <iostream>

Window::Window(uint32_t width, uint32_t height)
{
    _width = width;
    _height = height;

    _background = Color::Black;

    _window = mfb_open_ex("Game", width, height, false);
    _buffer = (int*) malloc(width * height * sizeof(int));

    Input::Initialize(_window);
	Timer::Init();
    AudioManager::Init();

    // Assign each chars to an image using char value as index
    auto font = Image(ATLAS_FONT_IMAGE_PATH);
    _chars = {};
    _charsImage = {};

    for (rtpAtlasSprite sprite: rtpDescBoldFonts)
    {
        _chars[sprite.charValue] = sprite;
        _charsImage[sprite.charValue] = font.Cut(sprite.positionX, sprite.positionY, sprite.sourceWidth, sprite.sourceHeight);
    }

    Logger::Log("Window created");
}

void Window::Update()
{
    Input::Update();
	Timer::Update();

    int state = mfb_update_ex(_window, _buffer, _width, _height);

    if (state < 0)
    {
        _window = nullptr;
    }
    else
    {
        memset(_buffer, 0, _width * _height * sizeof(int));

        // Draw background
        for (uint32_t i = 0; i < _width * _height; i++)
        {
            _buffer[i] = _background;
        }
    }

	_frame++;
}

bool Window::IsOpen()
{
    return _window != nullptr;
}

void Window::DrawPixel(uint32_t index, int color)
{
    if (index >= _width * _height || color >> 24 == 0)
    {
        return;
    }

    // Alpha blending
    const float srcAlpha = (float) ((uint8_t) (color >> 24)) / 255.f;
    const float destAlpha = 1.f - srcAlpha;
    const int destColor = _buffer[index];

    const auto destR = (uint8_t) (destColor >> 16);
    const auto destG = (uint8_t) (destColor >> 8);
    const auto destB = (uint8_t) (destColor);

    const auto srcR = (uint8_t) (color >> 16);
    const auto srcG = (uint8_t) (color >> 8);
    const auto srcB = (uint8_t) (color);

    const auto r = (uint8_t) (srcR * srcAlpha + destR * destAlpha);
    const auto g = (uint8_t) (srcG * srcAlpha + destG * destAlpha);
    const auto b = (uint8_t) (srcB * srcAlpha + destB * destAlpha);

    color = MFB_RGB(r, g, b);

    _buffer[index] = color;
}

void Window::DrawPixel(uint32_t x, uint32_t y, int color)
{
	const uint32_t index = y * _width + x;

	if (x >= _width || y >= _height)
	{
		return;
	}

	DrawPixel(index, color);
}

void Window::DrawHorizontalLine(uint32_t x, uint32_t y, uint32_t length, int color)
{
    for (uint32_t i = 0; i < length; i++)
    {
        DrawPixel(x + i, y, color);
    }
}

void Window::DrawVerticalLine(uint32_t x, uint32_t y, uint32_t length, int color)
{
    for (uint32_t i = 0; i < length; i++)
    {
        DrawPixel(x, y + i, color);
    }
}

void Window::DrawRectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, int color)
{
    DrawHorizontalLine(x, y, width, color);
    DrawHorizontalLine(x, y + height - 1, width, color);

    DrawVerticalLine(x, y, height, color);
    DrawVerticalLine(x + width - 1, y, height, color);
}

void Window::DrawFullRectangle(uint32_t x, uint32_t y, uint32_t width, uint32_t height, int color)
{
    for (uint32_t i = 0; i < height; i++)
    {
        DrawHorizontalLine(x, y + i, width, color);
    }
}

void Window::DrawLine(Vector2I start, Vector2I end, int color)
{
    // Use breseham algorithm
    int dx = abs(end.X - start.X);
    int dy = abs(end.Y - start.Y);
    int sx = start.X < end.X ? 1 : -1;
    int sy = start.Y < end.Y ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        DrawPixel(start.X, start.Y, color);

        if (start.X == end.X && start.Y == end.Y)
        {
            break;
        }

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            start.X += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            start.Y += sy;
        }
    }
}

Vector2I Window::GetStartPosition(uint32_t width, uint32_t height, int x, int y, Pivot pivot)
{
    Vector2I position {};

	if (pivot == Pivot::Center)
	{
		position.X = x - width / 2;
		position.Y = y - height / 2;
	}
	else if (pivot == Pivot::TopLeft)
	{
		position.X = x;
		position.Y = y;
	}
	else if (pivot == Pivot::TopRight)
	{
		position.X = x - width;
		position.Y = y;
	}

	return position;
}

Vector2I Window::GetRotatedPosition(int x, int y, Image image, Pivot pivot)
{
    float angle = image.GetRotation();
    int width = image.GetWidth();
    int height = image.GetHeight();
    float sinAngle = sin(Utility::ToRadians(angle));
    float cosAngle = cos(Utility::ToRadians(angle));
    Vector2I position {};

    if (pivot == Pivot::Center)
    {
        x -= width / 2;
        y -= height / 2;

        position.X = (int) (x * cosAngle - y * sinAngle) + width / 2;
        position.Y = (int) (x * sinAngle + y * cosAngle) + height / 2;
    }
    else if (pivot == Pivot::TopLeft)
    {
        position.X = (int) (x * cosAngle - y * sinAngle);
        position.Y = (int) (x * sinAngle + y * cosAngle);
    }
	else if (pivot == Pivot::TopRight)
	{
		x -= width;

		position.X = (int) (x * cosAngle - y * sinAngle) + width;
		position.Y = (int) (x * sinAngle + y * cosAngle);
	}

    return position;
}

void Window::DrawImage(Image image, int x, int y, Pivot pivot)
{
	uint32_t imageWidth = image.GetWidth();
	uint32_t imageHeight = image.GetHeight();
    float imageRotation = image.GetRotation();

    auto position = GetStartPosition(imageWidth, imageHeight, x, y, pivot);

    for (uint32_t i = 0; i < imageHeight; i++)
    {
        for (uint32_t j = 0; j < imageWidth; j++)
        {
            int color = image.GetBuffer()[i * imageWidth + j];

            auto finalX = j;
            auto finalY = i;

            if (imageRotation != 0.f)
            {
                auto rotatedPosition = GetRotatedPosition(j, i, image, pivot);
                finalX = rotatedPosition.X;
                finalY = rotatedPosition.Y;
            }

            DrawPixel(position.X + finalX, position.Y + finalY, color);
        }
    }
}

void Window::DrawText(Text text)
{
    int length = text.Text.length();
    int width = 0, height = 0;
    float scale = text.Size / ATLAS_FONT_SIZE;

    for (uint32_t i = 0; i < length; i++)
    {
        char c = text.Text[i];

        if (_chars[c].charValue == 0)
        {
            c = '?';
        }

        width += _charsImage[c].GetWidth();

        if (height < _charsImage[c].GetHeight())
        {
            height = _charsImage[c].GetHeight();
        }
    }

    auto position = GetStartPosition(width, height, text.Position.X, text.Position.Y, text.Pivot);

    for (uint32_t i = 0; i < length; i++)
    {
        char c = text.Text[i];

        if (_chars[c].charValue == 0)
        {
            c = '?';
        }

        _charsImage[c].SetScale(scale);

        if (text.Shadow)
        {
            _charsImage[c].SetColor(Color::Black);

            const int shadowOffset = 2;

            DrawImage(_charsImage[c],
                      position.X + _chars[c].charOffsetX + shadowOffset, position.Y + _chars[c].charOffsetY + shadowOffset, text.Pivot);
        }

        _charsImage[c].SetColor(Color::White);

        DrawImage(_charsImage[c], position.X + _chars[c].charOffsetX, position.Y + _chars[c].charOffsetY, text.Pivot);

        position.X += _chars[c].charAdvanceX * scale + 1;
    }
}

Vector2I Window::GetMousePosition()
{
    Vector2I position {};

    position.X = mfb_get_mouse_x(_window);
    position.Y = mfb_get_mouse_y(_window);

    return position;
}