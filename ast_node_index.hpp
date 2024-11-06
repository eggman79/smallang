#ifndef AST_NODE_INDEX_HPP  
#define AST_NODE_INDEX_HPP

#include <cstdint>
#include <limits>

using AstNodeIndex = std::uint32_t;
static const AstNodeIndex UndefinedAstNodeIndex = std::numeric_limits<AstNodeIndex>::max();

#endif  //  AST_NODE_INDEX_HPP
