#ifndef AST_HPP
#define AST_HPP

#include <cstdint>
#include <vector>

using AstNodeIndex = std::uint32_t;

struct AstNode {
  enum class Kind {None, Type, Value, StructType, StructField, UnionType, UnionField};
  enum class TypeKind {i8, i16, i32, u8, u16, u32, f32, f64, Struct, Union, Fun};

  union {
    struct {
      AstNodeIndex type; 
    } value;

    struct {
      TypeKind type_kind;
    } type;

    struct {
      AstNodeIndex return_type;
      std::vector<AstNodeIndex>* param_types;
    } fun_type;

    struct {
    } struct_type;

    struct {
    
    } struct_field;
  };
};

#endif  // AST_HPP
