#ifndef AST_NODE_INDEX_HPP  
#define AST_NODE_INDEX_HPP

#include <cstdint>
#include <limits>

#include "strong_type.hpp"

struct AstNodePhantom {};
using AstNodeIndex = StrongType<std::uint32_t, AstNodePhantom>;
static const AstNodeIndex UndefinedAstNodeIndex(std::numeric_limits<AstNodeIndex>::max());

struct AstNodeIndexHash {
  std::size_t operator()(const AstNodeIndex index) const noexcept {
    return index.get();
  }
};

#endif  //  AST_NODE_INDEX_HPP
