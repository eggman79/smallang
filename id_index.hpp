#ifndef ID_INDEX_HPP
#define ID_INDEX_HPP

#include <cstdint>
#include <limits>

using IdIndex = uint32_t;
static const IdIndex UndefinedIdIndex = std::numeric_limits<IdIndex>::max();

#endif  // ID_INDEX_HPP


