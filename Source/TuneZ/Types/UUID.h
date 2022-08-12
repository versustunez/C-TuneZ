#pragma once

#include "TypeDefs.h"

#include <functional>

namespace TuneZ {

// "UUID" (universally unique identifier) or GUID is (usually) a 128-bit integer
// used to "uniquely" identify information. But this is currently okay as it is
// ;)

class UUID {
public:
  UUID();
  UUID(u64 uuid);
  UUID(const UUID &other);

  operator u64() { return m_UUID; }
  operator const u64() const { return m_UUID; }

private:
  uint64_t m_UUID;
};
} // namespace TuneZ

namespace std {

template <> struct hash<TuneZ::UUID> {
  std::size_t operator()(const TuneZ::UUID &uuid) const {
    return hash<uint64_t>()((uint64_t)uuid);
  }
};

} // namespace std