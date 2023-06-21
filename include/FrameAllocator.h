#pragma once

#include <cstdarg>

namespace FrameAllocator
{
    void Initialize(size_t capacity);
    void* Allocate(size_t size);
    void OnFrameEnd();
    char* Format(const char* format, ...);
    char* FormatV(const char* format, va_list args);
}