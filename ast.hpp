#ifndef AST_HPP
#define AST_HPP

#include "id_cache.hpp"
#include <cstdint>
#include <vector>

using AstNodeIndex = std::uint32_t;

struct AstNode {
  enum class Kind {None, Type, Value};
  enum class TypeKind {i8, i16, i32, u8, u16, u32, f32, f64, Struct, Union, Fun};
  enum class ValueKind {LocalVariable, GlobalVariable, StringLiteral};

  AstNode(Kind kind) : kind(kind) {}
  AstNode(const AstNode&) = delete;
  AstNode(AstNode&&) = default;
  AstNode& operator=(const AstNode&) = delete;
  AstNode& operator=(AstNode&&) = default;

  ~AstNode() {
  }

  Kind kind;

  union Node {
    struct Value {
      ValueKind value_kind;
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

    struct StringLiteral {
      Value value;
      IdIndex id_str;
    };

    struct Type {
      TypeKind type_kind;
    };
    Type type;

    struct {
      Type type;
      AstNodeIndex return_type;
      std::vector<AstNodeIndex>* param_types;
    } fun_type;

    struct {
    } struct_type;

    struct {

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
