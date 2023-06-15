#pragma once

namespace FrameAllocator
{
    void Initialize(size_t capacity);
    void* Allocate(size_t size);
    void OnFrameEnd();
}