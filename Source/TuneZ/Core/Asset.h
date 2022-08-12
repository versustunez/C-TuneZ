#pragma once
#include <TypeDefs.h>

namespace TuneZ {
class AssetManager;
typedef uint32_t AssetHandle;
class Asset {
public:
  AssetHandle handle() { return m_handle; }
  String& file() { return m_file; }
  template <class T>
  T* as() {
    return static_cast<T*>(this);
  }

  virtual void init() {};

  protected:
  friend AssetManager;
  AssetHandle m_handle{};
  String m_file;
};

class AssetManager : public Instance<AssetManager> {
public:
  AssetManager();
  Asset* asset(AssetHandle);
  Ref<Asset> assetRef(AssetHandle);
  void add(const Ref<Asset>& asset);
  void remove(AssetHandle);
  bool has(AssetHandle);

  AssetHandle findHandleByFile(const String& file);

  template <typename T, typename... Params>
  T *create(const String& file, Params... params) {
    static_assert(std::is_base_of<Asset, T>::value, "is not typeof Asset");
    auto comp = CreateRef<T>(params...);
    comp->m_file = file;
    comp->init();
    add(comp);
    return comp.get();
  }
  template <typename T, typename... Params>
  T *create(Params... params) {
    static_assert(std::is_base_of<Asset, T>::value, "is not typeof Asset");
    auto comp = CreateRef<T>(params...);
    comp->init();
    add(comp);
    return comp.get();
  }

  template <typename T, typename... Params>
  T *getOrCreate(const String& file, Params... params) {
    static_assert(std::is_base_of<Asset, T>::value, "is not typeof Asset");
    auto handle = findHandleByFile(file);
    if (handle == 0) {
      return create<T>(params...);
    }
    return asset(handle);
  }

  void dump();

protected:
  AssetHandle m_lastHandle{1};
  Map<AssetHandle, Ref<Asset>> m_assets;
};

class MeshManager : public Instance<MeshManager> {
public:
  MeshManager();
  void Add(const String& name, AssetHandle handle);
  Map<String, AssetHandle> Meshes;
  Map<AssetHandle, String> AssetToName;
};
} // namespace TuneZ