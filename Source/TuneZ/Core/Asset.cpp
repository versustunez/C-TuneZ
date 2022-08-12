#include "Asset.h"

#include <sstream>

namespace TuneZ {

AssetManager::AssetManager() {
    add(CreateRef<Asset>());
}
Asset *AssetManager::asset(AssetHandle handle) {
  if (has(handle))
    return m_assets[handle].get();
  return nullptr;
}

Ref<Asset> AssetManager::assetRef(AssetHandle handle) {
  return m_assets[handle];
}

void AssetManager::add(const Ref<Asset>& asset) {
  asset->m_handle = ++m_lastHandle;
  m_assets[asset->m_handle] = asset;
}

void AssetManager::remove(AssetHandle handle) {
  if (has(handle))
    m_assets.erase(handle);
}

bool AssetManager::has(AssetHandle handle) {
  return m_assets.find(handle) != m_assets.end();
}

AssetHandle AssetManager::findHandleByFile(const String &file) {
  for (auto &item : m_assets) {
    if (item.second->file() == file)
      return item.first;
  }
  return 0;
}

void AssetManager::dump() {
  std::stringstream str{};
  str << "AssetManager Dump\n";
  for (auto &item : m_assets) {
    str << "Handle: " << item.first << " | File: " << item.second->file().c_str()
        << "\n";
  }
  puts(str.str().c_str());
}

MeshManager::MeshManager() {
  Add("None", 0);
}

void MeshManager::Add(const String &name, TuneZ::AssetHandle handle) {
    Meshes[name] = handle;
    AssetToName[handle] = name;
}
} // namespace TuneZ