#pragma once

#include <TypeDefs.h>

namespace TuneZ {
struct ConfigItem {
  double Value{0.0};
  explicit operator double() const { return Value;}
  explicit operator bool() const { return Value >= 0.5;}
};

class Config : public Instance<Config> {
public:
  ConfigItem* GetItem(const String& name);
  ConfigItem* GetItemOrDefault(const String& name, double value);
  String Dump();
  void Restore(const String& data);
protected:
  Map<String, ConfigItem> m_Items;
};
} // namespace TuneZ