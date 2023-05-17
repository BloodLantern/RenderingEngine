#include "core/debug/logger.hpp"

#include <iostream>

#include <windows.h>

#define ANSI_BEGIN          "\x1b["
#define ANSI_SEPARATOR      ";"

#define ANSI_COLOR_GREEN    "32m"
#define ANSI_COLOR_YELLOW   "33m"
#define ANSI_COLOR_RED      "31m"

#define ANSI_STYLE_BOLD     "1m"

#define ANSI_RESET          "0m"

std::ofstream Logger::mFile;
TsQueue<std::pair<std::string, Logger::LogLevel>> Logger::mLines;
std::condition_variable Logger::mCv;
Logger Logger::mInstance;

std::thread thread;
std::mutex mutex;

void Logger::OpenFile(const std::filesystem::path &filename)
{
    CloseFile();
    mFile.open(filename, std::ios_base::out | std::ios_base::app | std::ios_base::noreplace);
}

void Logger::CloseFile()
{
    if (mFile.is_open())
        mFile.close();
}

Logger::Logger()
{
    thread = std::thread(&Logger::Run, this);
}

Logger::~Logger()
{
    mRunning = false;
    if (thread.joinable())
        thread.join();
}

void Logger::Run()
{
    std::unique_lock<std::mutex> lock(mutex);
    while (mRunning)
    {
        while (!mLines.Empty())
            Log(mLines.Pop());
        mCv.wait(lock, [this] { return !mLines.Empty(); });
    }
}

void Logger::Log(const LogEntry& entry)
{
    const std::string& baseText = entry.first;
    LogLevel level = entry.second;
    const bool vsOutput = (int) level & VS_OUTPUT_LOG_BIT;

    if (vsOutput)
        level = LOG_LEVEL_BINARY_OP(level, ~VS_OUTPUT_LOG_BIT, &);

    if (mFile.is_open())
        mFile << baseText;

    std::string text;
    switch (level)
    {
        case LogLevel::Debug:
            text = ANSI_BEGIN ANSI_COLOR_GREEN + baseText;
            break;
        case LogLevel::Info:
            text = ANSI_BEGIN ANSI_RESET + baseText;
            break;
        case LogLevel::Warning:
            text = ANSI_BEGIN ANSI_COLOR_YELLOW + baseText;
            break;
        case LogLevel::Error:
            text = ANSI_BEGIN ANSI_COLOR_RED + baseText;
            break;
        case LogLevel::Fatal:
            text = ANSI_BEGIN ANSI_COLOR_RED ANSI_SEPARATOR ANSI_STYLE_BOLD + baseText;
            break;
    }

    std::cout << text;

    if (vsOutput)
        OutputDebugStringA(text.c_str());
}
