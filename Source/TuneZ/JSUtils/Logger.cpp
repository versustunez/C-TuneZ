#include "Logger.h"

#include "LogJS.h"

namespace TuneZ {
void Logger::Info(const std::string& message, const std::string& module) {
  pretty_log("log", module.c_str(), message.c_str());
}
void Logger::Warn(const std::string& message, const std::string& module) {
  pretty_log("warn", module.c_str(), message.c_str());
}
void Logger::Error(const std::string& message, const std::string& module) {
  pretty_log("error", module.c_str(), message.c_str());
}
void Logger::Debug(const std::string& message, const std::string& module) {
  pretty_log("debug", module.c_str(), message.c_str());
}

std::string Logger::ExtractModule(const std::string &prettyFunction) {
  size_t colons = prettyFunction.find("::");
  size_t begin = prettyFunction.substr(0,colons).rfind(' ') + 1;
  size_t end = prettyFunction.rfind('(') - begin;

  return prettyFunction.substr(begin,end);
}
} // namespace TuneZ