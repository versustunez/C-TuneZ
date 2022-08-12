#pragma once

#include <FMT.h>
#include <string>

#define LOCATION TuneZ::Logger::ExtractModule(__PRETTY_FUNCTION__)

#define VDEBUG(log, ...)                                                         \
  TuneZ::Logger::Debug(TuneZ::Logger::format(log, __VA_ARGS__), LOCATION)

#define VLOG(log, ...)                                                         \
  TuneZ::Logger::Info(TuneZ::Logger::format(log, __VA_ARGS__), LOCATION)

#define VWARN(log, ...)                                                        \
  TuneZ::Logger::Warn(TuneZ::Logger::format(log, __VA_ARGS__), LOCATION)

#define VERR(log, ...)                                                        \
  TuneZ::Logger::Error(TuneZ::Logger::format(log, __VA_ARGS__), LOCATION)

#define ERROR(...) VERR(__VA_ARGS__, "")
#define LOG(...) VLOG(__VA_ARGS__, "")
#define WARN(...) VWARN(__VA_ARGS__, "")
#define DBG(...) VDEBUG(__VA_ARGS__, "")

namespace TuneZ {
class Logger {
public:
  static void Info(const std::string &message,
                   const std::string &module = "WASM");
  static void Error(const std::string &message,
                    const std::string &module = "WASM");
  static void Warn(const std::string &message,
                   const std::string &module = "WASM");
  static void Debug(const std::string &message,
                    const std::string &module = "WASM");

  static std::string ExtractModule(const std::string &prettyFunction);
  template <typename S, typename... Args>
  static std::string format(const S &format, Args &&...args) {
    return fmt::format(format, args...);
  }
};
}; // namespace TuneZ