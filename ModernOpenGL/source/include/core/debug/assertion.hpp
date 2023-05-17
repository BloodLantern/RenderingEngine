#pragma once

#include "core/debug/logger.hpp"

#if _DEBUG
    #define ASSERT(cond, msg) {                                                 \
        if constexpr (!(cond))                                                  \
        {                                                                       \
            Logger::WarnVS("Debug assertion failed: " << msg << std::endl);     \
            __debugbreak();                                                     \
        }                                                                       \
    }
#else
    #define ASSERT(cond, msg) {                                                 \
        if constexpr (!(cond))                                                  \
        {                                                                       \
            Logger::ErrorVS("Release assertion failed: " << msg << std::endl);  \
            throw std::runtime_error("Release assertion failed.");              \
        }                                                                       \
    }
#endif
