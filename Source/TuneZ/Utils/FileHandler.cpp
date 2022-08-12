#include "FileHandler.h"
#include <JSUtils/Logger.h>
#include <TypeDefs.h>

#include <filesystem>
#include <fstream>
#include <unistd.h>
#define cd chdir

namespace TuneZ {
String FileHandler::readFile(const char *file) {
  std::string result;
  std::ifstream in(file, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    if (size != -1) {
      result.resize(size);
      in.seekg(0, std::ios::beg);
      in.read(&result[0], size);
    } else {
      ERROR("Cannot read file: {}", file);
    }
  } else {
    ERROR("Cannot open file: {}", file);
  }
  return {result};
}
String FileHandler::getFileName(const char *name) {
  auto p = std::filesystem::path(name);
  return String(p.filename().replace_extension(""));
}
String FileHandler::getExtension(const char *name) {
  auto p = std::filesystem::path(name);
  return String(p.extension());
}

bool FileHandler::changeWorkingDir(const char *dir) { return cd(dir) == 0; }
} // namespace TuneZ