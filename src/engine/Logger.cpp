#include "engine/Logger.h"

#include <iostream>

namespace Logger
{
    void Log(std::string message)
    {
        std::cout << message << std::endl;
    }

    void LogError(std::string message)
    {
        std::cout << "[ERROR] " << message << std::endl;
    }
}