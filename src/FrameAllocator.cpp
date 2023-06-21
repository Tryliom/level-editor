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

    char* Format(const char* format, ...)
    {
        va_list args;
        va_start(args, format); // <- this says: the variable argument list starts after the argument "format"

        size_t remaining = allocatedMemory - usedMemory;
        char* buffer = (char*) memory + usedMemory;

        int len = vsnprintf(buffer, remaining, format, args); // use the vsnprintf version, to which we can pass a va_list

        usedMemory += len + 1;

        va_end(args);

        if (len + 1 > remaining) {
            LOG("FrameAllocator: Not enough memory");
            exit(1);
        }

        return buffer;
    }

    char* FormatV(const char* format, va_list args)
    {
        size_t remaining = allocatedMemory - usedMemory;
        char* buffer = (char*) memory + usedMemory;

        int len = vsnprintf(buffer, remaining, format, args); // use the vsnprintf version, to which we can pass a va_list

        usedMemory += len + 1;

        if (len + 1 > remaining) {
            LOG("FrameAllocator: Not enough memory");
            exit(1);
        }

        return buffer;
    }
}