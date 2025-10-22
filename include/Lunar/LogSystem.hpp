#pragma once
#include <iostream>
#include <source_location>
#include <string_view>
#include <format>
#include <string>
#include <typeinfo>

#if defined(_MSC_VER)
    #include <Windows.h>
    #include <Dbghelp.h>
    #pragma comment(lib, "Dbghelp.lib")
#elif defined(__GNUG__)
    #include <cxxabi.h>
    #include <cstdlib>
#endif

namespace Lunar
{
    inline std::string DemangleTypeName(const char *name)
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
}

namespace Lunar
{
    /// @enum LogLevel
    /// @brief Verbosity & severity levels.
    enum class LogLevel
    {
        Trace,  /// Diagnostic, detailed trace info.
        Info,   /// General information.
        Warn,   /// Non-fatal warnings that may need attention.
        Error,  /// Errors that may prevent typical functions.
        Fatal,  /// Fatal errors that tend to stop execution.
    };

    /// @brief Humanizes and stringifies log levels
    /// @param lvl Log level to stringify
    /// @return A constexpr string view representing the level
    inline constexpr std::string_view ToString(LogLevel lvl)
    {
        switch (lvl)
        {
            case LogLevel::Trace: return "TRACE";
            case LogLevel::Info:  return "INFO";
            case LogLevel::Warn:  return "WARN";
            case LogLevel::Error: return "ERROR";
            case LogLevel::Fatal: return "FATAL";
            default:              return "UNKNOWN";
        }
    }

    /**
     *  @brief Type-tagged logger.
     *
     * The template parameter @p T is typically owning class or subsys,
     * allowing log entries to automatically identify origin.
     * 
     * Example:
     * @code
     * struct Renderer {};
     * Lunar::Logger<Renderer>::Info("Loaded {} textures",count);
     * @endcode
     */
    template <typename T>
    class Logger
    {
    public:
        /**
         * @brief Generic log entrypoint.
         * 
         * Formats a message with @c std::vformat and writes it to @c std::cerr.
         * If a formatting error occurs, it reports it using @c [LOG ERR].
         *
         * @tparam Args Variadic argument types passed to the format string.
         * @param level   Logging severity.
         * @param formatted_str  Format string (runtime or compile-time literal).
         * @param args    Arguments for the format string.
         * @param loc     Source location automatically supplied by the compiler.
         */
        template <typename... Args>
        static void Log(LogLevel level, std::string_view formatted_str, Args&&... args,
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
                std::cerr << std::format("[LOG ERR] Badly formatted string: {}\n", e.what());
            }
            
        }

        /** @brief Logs a trace-level message */
        template <typename... Args>
        static void Trace(std::string_view fmtStr, Args&&... args,
                          const std::source_location& loc = std::source_location::current())
        {
            Log(LogLevel::Trace, fmtStr, std::forward<Args>(args)..., loc);
        }

        /** @brief Logs an info-level message */
        template <typename... Args>
        static void Info(std::string_view fmtStr, Args&&... args,
                         const std::source_location& loc = std::source_location::current())
        {
            Log(LogLevel::Info, fmtStr, std::forward<Args>(args)..., loc);
        }

        /** @brief Logs a warning-level message */
        template <typename... Args>
        static void Warn(std::string_view fmtStr, Args&&... args,
                         const std::source_location& loc = std::source_location::current())
        {
            Log(LogLevel::Warn, fmtStr, std::forward<Args>(args)..., loc);
        }

        /** @brief Logs an error-level message */
        template <typename... Args>
        static void Error(std::string_view fmtStr, Args&&... args,
                          const std::source_location& loc = std::source_location::current())
        {
            Log(LogLevel::Error, fmtStr, std::forward<Args>(args)..., loc);
        }

        /** @brief Logs a fatal-level message */
        template <typename... Args>
        static void Fatal(std::string_view fmtStr, Args&&... args,
                          const std::source_location& loc = std::source_location::current())
        {
            Log(LogLevel::Fatal, fmtStr, std::forward<Args>(args)..., loc);
        }
    };
}