#pragma once

#include <cstdint>

class Image
{
public:
    Image();
    Image(uint32_t width, uint32_t height, uint32_t* buffer);
	explicit Image(const char* filename);

private:
	uint32_t _originalWidth;
	uint32_t _originalHeight;

    int* _originalBuffer;

	uint32_t _width;
	uint32_t _height;

    int* _buffer;

	float _scale = 1.f;
    float _rotation = 0.f;
	uint32_t _color = 0xFFFFFFFF;

	void ApplyColor();

public:
	[[nodiscard]] uint32_t GetWidth() const { return _width; }
	[[nodiscard]] uint32_t GetHeight() const { return _height; }

    int* GetBuffer() { return _buffer; }

	[[nodiscard]] float GetScale() const { return _scale; }
    [[nodiscard]] float GetRotation() const { return _rotation; }

    void SetRotation(float angle);
	void SetScale(float factor);
    void SetColor(int color);

    Image Cut(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
};