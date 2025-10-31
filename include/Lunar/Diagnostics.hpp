#pragma once

#include <string>
#include <string_view>
#include <cstdlib>
#include <iostream>
#include <format>
#include <source_location>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <thread>

#if !defined(_WIN32) && !defined(_WIN64)
    #include <cxxabi.h>
#else
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <Dbghelp.h>
    #pragma comment(lib, "Dbghelp.lib")
#endif

/** Lunar Diagnostics Module
 * 
 * Provides diagnostic and platform/build information for Lunar.
 * This header is fully self-contained and cross-platform.
 * 
 * Includes the INTERNAL log submodule used for development diagnostics.
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
    #define LUNAR_ARCH "Unknown"
#endif

namespace Lunar::Diag
{
    /// @brief Get the current platform string.
    static constexpr const char *GetPlatform() { return LUNAR_PLATFORM; }

    /// @brief Get the current architecture string.
    static constexpr const char *GetArch() { return LUNAR_ARCH; }

    /// @brief Get the current build mode.
    static constexpr const char *GetBuildMode()
    {
    #if defined(DBG_BUILD)
        return "Debug";
    #else
        return "Release";
    #endif
    }

    /// @brief Get the compiler name.
    static constexpr const char *GetCompiler()
    {
    #if defined(_MSC_VER)
        return "MSVC";
    #elif defined(__clang__)
        return "Clang";
    #elif defined(__GNUC__)
        return "GCC";
    #else
        return "Unknown Compiler";
    #endif
    }
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

    enum class ILogLevel
    {
        ITrace,
        IInfo,
        IWarn,
        IError,
        IFatal,
    };

    inline constexpr std::string_view ToStr(ILogLevel lvl)
    {
        switch (lvl)
        {
            case ILogLevel::ITrace: return "Internal TRACE";
            case ILogLevel::IInfo:  return "Internal INFO";
            case ILogLevel::IWarn:  return "Internal WARN";
            case ILogLevel::IError: return "Internal ERROR";
            case ILogLevel::IFatal: return "Internal FATAL";
            default:                return "Internal UNKNOWN";
        }
    }

    template<typename T>
    class Logger
    {
    private:
        inline static std::mutex log_mutex; // ensures thread-safe output
        inline static std::ostream* out_stream = &std::cerr; // customizable target stream

    public:
        /// @brief Redirects internal log output.
        static void SetOutput(std::ostream& stream)
        {
            std::scoped_lock lock(log_mutex);
            out_stream = &stream;
        }

        template <typename... Args>
        static void ILog(ILogLevel level, std::string_view fmt_str, Args&&... args,
                         const std::source_location& loc = std::source_location::current())
        {
            try
            {
                auto msg = std::vformat(fmt_str, std::make_format_args(std::forward<Args>(args)...));

                // Timestamp info 
                auto now = std::chrono::system_clock::now();
                auto tt  = std::chrono::system_clock::to_time_t(now);
                std::tm local_tm{};
            #if defined(_WIN32)
                localtime_s(&local_tm, &tt);
            #else
                localtime_r(&tt, &local_tm);
            #endif

                std::ostringstream oss;
                oss << std::put_time(&local_tm, "%F %T");

                // Thread ID
                auto tid = std::this_thread::get_id();

                std::scoped_lock lock(log_mutex);
                (*out_stream) << std::format(
                    "[{}] [{}] [{}] [{}::{}] {} ({}:{})\n",
                    oss.str(),
                    ToStr(level),
                    tid,
                    Demangle(typeid(T).name()),
                    loc.function_name(),
                    msg,
                    loc.file_name(),
                    loc.line());
            }
            catch (const std::format_error& e)
            {
                std::scoped_lock lock(log_mutex);
                (*out_stream) << std::format("[ILOG ERROR] Bad format: {}\n", e.what());
            }
        }
    };

    // The macro of endless convenience
    #define LUNAR_ILOG(Type, Level, Fmt, ...) \
        ::Lunar::InternalLog::Logger<Type>::ILog(Level, Fmt __VA_OPT__(, ) __VA_ARGS__)
}
