#include <iostream>

#include "core/debug/assertion.hpp"
#include "core/debug/logger.hpp"

int main()
{
    using namespace std::chrono_literals;
    Logger::OpenDefaultFile();
    Logger::LogInfo("Test");
    Logger::Stop();

    return 0;
}
