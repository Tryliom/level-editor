#include "FrameAllocator.h"
#include "Logger.h"

uint8_t* memory;
size_t usedMemory;
size_t allocatedMemory;

namespace FrameAllocator
{
    void Initialize(size_t capacity)
    {
        memory = (uint8_t*) malloc(capacity);
        usedMemory = 0;
        allocatedMemory = capacity;
    }

    void* Allocate(size_t size)
    {
        if (usedMemory + size > allocatedMemory)
        {
            LOG("FrameAllocator: Not enough memory");
            return nullptr;
        }

        void* result = memory + usedMemory;
        usedMemory += size;

        return result;
    }

    void OnFrameEnd()
    {
        usedMemory = 0;
    }
}