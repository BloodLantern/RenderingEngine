#include "core/debug/logger.hpp"

#include <iostream>

#include <windows.h>

#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_RED      "\x1b[31m"

#define ANSI_STYLE_BOLD     "\x1b[1m"

#define ANSI_RESET          "\x1b[0m"

TsQueue<Logger::LogEntry> Logger::mLines;
std::ofstream Logger::mFile;
std::condition_variable Logger::mCondVar;

std::thread Logger::thread = std::thread(&Logger::Run);
std::mutex mutex;
bool synchronizing = false;
bool running = true;

void Logger::OpenFile(const std::filesystem::path &filename)
{
    CloseFile();
    const bool exists = std::filesystem::exists(filename);
    mFile.open(filename, std::ios_base::out | std::ios_base::app);

    if (!mFile.good())
    {
        LogWarning("Could not open log file for writing: " + std::filesystem::absolute(filename).string());
        return;
    }

    // If the file already exists, add newlines to space from the last log
    if (exists)
        mFile << std::endl;
}

void Logger::OpenDefaultFile()
{
    // Get the current date and format it in yyyy-mm-dd for the file name
    const std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltime;
    localtime_s(&ltime, &t);
    const std::_Timeobj<char, const tm *> timeFormatter = std::put_time(&ltime, "%F.log");
    const std::string date = (std::ostringstream() << timeFormatter).str();
    OpenFile("logs/" + date);
}

void Logger::CloseFile()
{
    if (mFile.is_open())
    {
        mFile.flush();
        mFile.close();
    }
}

void Logger::Synchronize()
{
    if (!mLines.Empty())
    {
        synchronizing = true;
        mCondVar.notify_one();
        std::unique_lock<std::mutex> lock(mutex);
        mCondVar.wait(lock, [] { return !synchronizing; });
    }
}

void Logger::Stop()
{
    Logger::Synchronize();
    running = false;
    mCondVar.notify_one();
    if (thread.joinable())
        thread.join();
}

void Logger::Run()
{
    // Set thread name for easier debugging
    SetThreadDescription(thread.native_handle(), L"Logger Thread");
    std::unique_lock<std::mutex> lock(mutex);
    while (running)
    {
        mCondVar.wait(lock, [] { return !mLines.Empty() || !running || synchronizing; });

        while (!mLines.Empty())
            Log(mLines.Pop());

        // As we don't use std::endl for newlines, make sure to flush the streams before going to sleep
        std::cout.flush();
        if (mFile.is_open())
            mFile.flush();

        if (synchronizing)
        {
            synchronizing = false;
            mCondVar.notify_one();
        }
    }
    CloseFile();
}

void Logger::Log(const LogEntry& entry)
{
    // Get the message time and format it in [hh:mm:ss]
    const std::time_t t = std::chrono::system_clock::to_time_t(entry.time);
    std::tm ltime;
    localtime_s(&ltime, &t);
    const std::_Timeobj<char, const tm *> timeFormatter = std::put_time(&ltime, "[%H:%M:%S] ");
    const std::string time = (std::ostringstream() << timeFormatter).str();

    // Setup the base text message
    const std::string& baseMessage = entry.message + '\n', coloredBaseMessage = time + baseMessage;
    LogLevel level = entry.level;
    
    const bool outputToVS = (int) level & VS_OUTPUT_LOG_BIT;
    if (outputToVS)
        level = LOG_LEVEL_BINARY_OP(level, ~VS_OUTPUT_LOG_BIT, &);

    std::string coloredMessage, uncoloredMessage;
    switch (level)
    {
        case LogLevel::Info:
            coloredMessage = coloredBaseMessage;
            uncoloredMessage = time + "[INFO] " + baseMessage;
            break;
        case LogLevel::Warning:
            coloredMessage = ANSI_COLOR_YELLOW + coloredBaseMessage + ANSI_RESET;
            uncoloredMessage = time + "[WARN] " + baseMessage;
            break;
        case LogLevel::Error:
            coloredMessage = ANSI_COLOR_RED + coloredBaseMessage + ANSI_RESET;
            uncoloredMessage = time + "[ERROR] " + baseMessage;
            break;
        case LogLevel::Fatal:
            coloredMessage = ANSI_STYLE_BOLD ANSI_COLOR_RED + coloredBaseMessage + ANSI_RESET;
            uncoloredMessage = time + "[FATAL] " + baseMessage;
            break;
    }

    std::cout << coloredMessage;

    if (mFile.is_open())
        mFile << uncoloredMessage;

    if (outputToVS)
        OutputDebugStringA(uncoloredMessage.c_str());
}
