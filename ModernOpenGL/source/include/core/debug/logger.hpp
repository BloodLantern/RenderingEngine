#pragma once

#include <filesystem>
#include <fstream>
#include <thread>
#include <condition_variable>
#include <utility>

#include "core/data_structure/tsqueue.hpp"

#define DEBUG_LOG(str) Logger::InfoVS(__FILE__ + "(" + __LINE__ + "): " + str)
#define LOG_LEVEL_BINARY_OP(left, right, operator) (static_cast<Logger::LogLevel>(static_cast<unsigned char>(left) operator right))

class Logger
{
public:
    enum class LogLevel : unsigned char
    {
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    template<class... Args>
    static void Info(const std::string& format, Args&&... args)
    {
        mLines.Push(LogEntry(Format(format, std::forward<Args>(args)...), LogLevel::Info));
        mCv.notify_one();
    }
    template<class... Args>
    static void InfoVS(const std::string& format, Args&&... args)
    {
        mLines.Push(LogEntry(Format(format, std::forward<Args>(args)...), LOG_LEVEL_BINARY_OP(LogLevel::Info, VS_OUTPUT_LOG_BIT, |)));
        mCv.notify_one();
    }

    static void OpenFile(const std::filesystem::path& filename);
    static bool HasFileOpen() { return mFile.is_open(); }
    static void CloseFile();

private:
    using LogEntry = std::pair<std::string, LogLevel>;

    static constexpr const unsigned char VS_OUTPUT_LOG_BIT = 0b10000000;

    static std::ofstream mFile;
    static TsQueue<LogEntry> mLines;
    static std::condition_variable mCv;
    static Logger mInstance;

    bool mRunning = true;

    Logger();
    ~Logger();

    void Run();
    void Log(const LogEntry& entry);

    template<class... Args>
    static inline std::string Format(const std::string& format, Args&&... args)
    {
        std::string buffer = std::string(0x800, '\0');
        sprintf_s(buffer.data(), buffer.size(), format.c_str(), std::forward<Args>(args)...);
        return buffer;
    }
};
