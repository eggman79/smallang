#ifndef AST_HPP
#define AST_HPP

#include "id_cache.hpp"
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <vector>

using AstNodeIndex = std::uint32_t;
static const AstNodeIndex UndefinedAstNodeIndex = std::numeric_limits<AstNodeIndex>::max();

class OrderedDict {
public:
  OrderedDict(IdCache& id_cache) : m_id_cache(id_cache) {}
  OrderedDict(const OrderedDict&) = delete;
  OrderedDict(OrderedDict&&) = delete;
  OrderedDict& operator=(const OrderedDict&) = delete;
  OrderedDict& operator=(OrderedDict&&) = delete;

  void append(const char* key, uint32_t key_length, AstNodeIndex node_index) {
    auto idx_key = m_id_cache.get(key, key_length);
    append(idx_key, node_index);
  }

  void append(IdIndex name, AstNodeIndex node_index) {
    auto it = m_map.find(name);
    if (it == m_map.end()) {
      m_map.emplace(name, node_index);
      m_nodes.emplace_back(node_index);
    }
  }

  void append(AstNodeIndex node_index) {
    m_nodes.emplace_back(node_index);
  }

  AstNodeIndex find(IdIndex name) const {
    auto it = m_map.find(name);

    if (it == m_map.end()) {
      return UndefinedAstNodeIndex;
    }
    return it->second;
  }

  const std::vector<AstNodeIndex>& get_nodes() const { return m_nodes;}

private:
  IdCache& m_id_cache;
  using Map = std::unordered_map<IdIndex, AstNodeIndex> ;
  Map m_map;
  std::vector<AstNodeIndex> m_nodes;
};

struct AstNode {
  enum class Kind {
    None, Type, Value, Statement, 
    I8Type, I16Type, I32Type, U8Type, U16Type, U32Type, F32Type, F64Type, 
    StructType, UnionType,
    FunType, LocalVariable, GlobalVariable, StringLiteral, 
    I8Literal, I16Literal, I32Literal, U8Literal, U16Literal, U32Literal, 
    F32Literal, F64Literal,
    Expression,
    Scope,
  };
  enum class StatementKind {};

  AstNode(Kind kind) : kind(kind) {}
  AstNode(const AstNode&) = delete;
  AstNode(AstNode&&) = default;
  AstNode& operator=(const AstNode&) = delete;
  AstNode& operator=(AstNode&&) = default;

  static bool is_type(AstNode::Kind kind) {
    switch (kind) {
      case AstNode::Kind::I8Type: 
      case AstNode::Kind::I16Type: 
      case AstNode::Kind::I32Type: 
      case AstNode::Kind::U8Type: 
      case AstNode::Kind::U16Type: 
      case AstNode::Kind::U32Type: 
      case AstNode::Kind::F32Type:
      case AstNode::Kind::F64Type:
      case AstNode::Kind::StructType:
      case AstNode::Kind::UnionType:
      case AstNode::Kind::FunType:
        return true;
      default: 
        return false;
    }
  } 

  static bool is_value(AstNode::Kind kind) {
    switch (kind) {
      case AstNode::Kind::LocalVariable:
      case AstNode::Kind::GlobalVariable:
      case AstNode::Kind::StringLiteral:
      case AstNode::Kind::I8Literal:
      case AstNode::Kind::I16Literal:
      case AstNode::Kind::I32Literal:
      case AstNode::Kind::U8Literal:
      case AstNode::Kind::U16Literal:
      case AstNode::Kind::U32Literal:
      case AstNode::Kind::F32Literal:
      case AstNode::Kind::F64Literal:
      case AstNode::Kind::Expression:
        return true;
      default:
        return false;
    }
  }

  ~AstNode() {
  }

  Kind kind;

  union Node {
    struct Value {
      AstNodeIndex type; 
    };

    Value value;

    struct {
      Value value;
      IdIndex id;
    } global_variable;

    struct {
      Value value;
      IdIndex id;
    } local_variable;

    struct {
      AstNodeIndex left;
      AstNodeIndex right;
    } expression;

    struct StringLiteral {
      Value value;
      IdIndex id_str;
    };

    template <typename Type>
    struct NumberLiteral {
      Value value;
      Type literal_value;
    };

    StringLiteral string_literal;
    NumberLiteral<int8_t> i8_literal;
    NumberLiteral<int16_t> i16_literal;
    NumberLiteral<int32_t> i32_literal;
    NumberLiteral<int64_t> i64_literal;
    NumberLiteral<uint8_t> u8_literal;
    NumberLiteral<uint16_t> u16_literal;
    NumberLiteral<uint32_t> u32_literal;

    struct FunType {
      AstNodeIndex return_type;
      std::vector<AstNodeIndex>* param_types;
      IdIndex name;
    };
    FunType fun_type;

    struct {
      IdIndex name;
      std::vector<AstNodeIndex>* fields;
    } struct_type;

    struct {
      IdIndex name;
      std::vector<AstNodeIndex>* fields;
    } struct_field;
  };
  Node node;
};

class Ast {
public:
  AstNode& get_node(AstNodeIndex index) {
    return nodes[index];
  }

  AstNodeIndex create(AstNode::Kind kind) {
    nodes.emplace_back(kind);
    return nodes.size() - 1;
  }
private:
  std::vector<AstNode> nodes;
};

#endif  // AST_HPP
