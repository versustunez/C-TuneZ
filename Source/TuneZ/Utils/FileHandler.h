#pragma once

#include <TypeDefs.h>

namespace TuneZ {
class FileHandler {
public:
  static String readFile(const char* file);
  static String getFileName(const char* name);
  static String getExtension(const char* name);
  static bool changeWorkingDir(const char* dir);
};
} // namespace VUtils