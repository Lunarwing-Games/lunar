#pragma once

#include <string>
#include <cxxabi.h>
#include <cstdlib>
#include <string_view>

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <Dbghelp.h>
    #pragma comment(lib, "Dbghelp.lib")
#endif

/** Lunar Diagnostics Module
 * 
 * This module provides important diagnostic features for Lunar, including
 * platform detection and build configuration information.
 * 
 * It is included by default in the main Lunar header.
 * This header is also self-contained.
 * 
 * This also contains an INTERNAL log submodule used for development.
 */

#if defined(DEBUG)
    #define DBG_BUILD 1
#endif

#if defined(_WIN32) || defined(_WIN64)
    #define LUNAR_PLATFORM "Win32"
#elif defined(__APPLE__)
    #define LUNAR_PLATFORM "MacOS"
#elif defined(__linux__)
    #define LUNAR_PLATFORM "Linux"
#else
    #warning "Unknown OS environment!"
    #define LUNAR_PLATFORM "Unknown"
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__)
    #define LUNAR_ARCH "x86_64"
#elif defined(__i386__) || defined(i386)
    #define LUNAR_ARCH "x86"
#elif defined(__aarch64__)
    #define LUNAR_ARCH "ARM64"
#elif defined(__arm__)
    #define LUNAR_ARCH "ARM"
#else
    #error "Unknown architecture!"
    #define LUNAR_ARCH "Unknown"
#endif

namespace Lunar::Diag
{
    /// @brief Get the current platform string.
    /// @return A string representing the current platform.
    static constexpr const char *GetPlatform() { return LUNAR_PLATFORM; }

    /// @brief Get the current architecture string.
    /// @return A string representing the current architecture.
    static constexpr const char *GetArch() { return LUNAR_ARCH; }
}

namespace Lunar::InternalLog
{
    inline std::string Demangle(const char *name)
    {
    #if defined(_MSC_VER)
        char buffer[512];
        if (UnDecorateSymbolName(name, buffer, sizeof(buffer), UNDNAME_COMPLETE))
            return buffer;
        return name;
    #elif defined(__GNUG__)
        int status = 0;
        char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        std::string result = (status == 0 && demangled) ? demangled : name;
        std::free(demangled);
        return result;
    #else
        return name;
    #endif
    }

    /// @brief Internal log levels (a mirror of external logs)
    enum class ILogLevel
    {
        ITrace,
        IInfo,
        IWarn,
        IError,
        IFatal,
    };

    /// @brief 
    /// @param lvl 
    /// @return 
    inline constexpr std::string_view ToStr(ILogLevel lvl)
    {
        switch (lvl)
        {
            case ILogLevel::ITrace: return "Internal TRACE";
            case ILogLevel::IInfo:  return "Internal INFO";
            case ILogLevel::IWarn:  return "Internal WARN";
            case ILogLevel::IError: return "Internal ERROR";
            case ILogLevel::IFatal: return "Internal FATAL";
            default:              return "Internal UNKNOWN";
        }
    }

    template<typename T>
    class Logger
    {
    public:
        template <typename... Args>
        static void ILog(ILogLevel level, std::string_view formatted_str, Args&&... args,
                        const std::source_location &loc = std::source_location::current())
        {
            try
            {
                auto msg = std::vformat(formatted_str, std::make_format_args(
                    std::forward<Args>(args)...
                ));

                std::cerr << std::format("[{}] [{}::{}] {} ({}:{})\n",
                    ToString(level),
                    DemangleTypeName(typeid(T).name()),
                    loc.function_name(),
                    msg,
                    loc.file_name(),
                    loc.line());
            }
            catch(const std::format_error& e)
            {
                std::cerr << std::format("[ILOG ERROR] Badly formatted string: {}\n", e.what());
            }
            
        }    
    };
}