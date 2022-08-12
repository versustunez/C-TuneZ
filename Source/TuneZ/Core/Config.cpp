#include "Config.h"

#include "JSUtils/Logger.h"

#include <Json/json.hpp>

namespace TuneZ {
ConfigItem *Config::GetItem(const String &name) { return &m_Items[name]; }
ConfigItem *Config::GetItemOrDefault(const String &name, double value) {
  auto isNew = m_Items.find(name) == m_Items.end();
  auto *item = &m_Items[name];
  if (isNew)
    item->Value = value;
  return item;
}

String Config::Dump() {
  nlohmann::json json;
  for (auto &item : m_Items) {
    json[item.first] = item.second.Value;
  }
  return json.dump(2);
}

void Config::Restore(const String &data) {
  auto jsonData = nlohmann::json::parse(data);
  for (auto &[key, value] : jsonData.items()) {
    m_Items[key] = {.Value = value};
  }
}
} // namespace TuneZ
