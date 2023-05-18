#pragma once

#include "core/debug/logger.hpp"

#if _DEBUG
    #define ASSERT(cond, msg) {                                     \
        if constexpr (!(cond))                                      \
        {                                                           \
            Logger::LogErrorToVS("Debug assertion failed: " msg);   \
            Logger::Synchronize();                                  \
            __debugbreak();                                         \
        }                                                           \
    }
#else
    #define ASSERT(cond, msg) {                                     \
        if constexpr (!(cond))                                      \
        {                                                           \
            Logger::LogFatalToVS("Release assertion failed: " msg); \
            Logger::Stop();                                         \
            throw std::runtime_error("Release assertion failed.");  \
        }                                                           \
    }
#endif
