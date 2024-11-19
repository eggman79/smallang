#ifndef ID_INDEX_HPP
#define ID_INDEX_HPP

#include <cstdint>
#include <limits>
#include "strong_type.hpp"

struct IdIndexPhantom {};
using IdIndex = StrongType<uint32_t, IdIndexPhantom>;

#endif  // ID_INDEX_HPP


