#ifndef ID_INDEX_HPP
#define ID_INDEX_HPP

#include <cstdint>
#include <limits>
#include "strong_type.hpp"

struct IdIndexPhantom {};
using IdIndex = StrongType<uint32_t, IdIndexPhantom>;
static const IdIndex UndefinedIdIndex = std::numeric_limits<IdIndex>::max();

struct IdIndexHash {
  std::size_t operator()(const IdIndex& index) const noexcept {
    return index.get();
  }
};


#endif  // ID_INDEX_HPP


