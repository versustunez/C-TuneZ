#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
template <typename T, typename S> using Map = std::unordered_map<T, S>;
template <typename T> using Vector = std::vector<T>;
template <typename T> using Queue = std::queue<T>;

template <typename T> class Instance {
public:
  static T &Get() {
    static T _instance;
    return _instance;
  }
};

typedef std::string String;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t s64;