#include <gtest/gtest.h>
#include <sstream>
#include <unordered_set>

#include "ast_node_index.hpp"
#include "id_index.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "id_cache.hpp"
#include "ir_value.hpp"
#include "strong_type.hpp"
#include "ir.hpp"

TEST(IdCache, Simple) {
  IdCache id_cache;
  auto index = id_cache.get("test", 4);
  EXPECT_EQ(index, id_cache.get("test", 4));
  EXPECT_STREQ(id_cache.get(index).str, "test");
  EXPECT_EQ(id_cache.get(index).length, 4);
}

TEST(Ast, Scope) {
  Ast ast;
  IdCache id_cache;
  auto struct_idx = ast.create(AstNode::Kind::Struct);
  auto& struct_node = ast[struct_idx];
  auto a_field_idx = ast.create(AstNode::Kind::StructField);
  auto& a_field = ast[a_field_idx];

  a_field.struct_field.value.type = ast.create(AstNode::Kind::I32Type);
  a_field.struct_field.name = id_cache.get("a_field");
  struct_node.struc.scope.add_node(a_field_idx, a_field.struct_field.name);
  {
    auto a_field_idx = ast[struct_idx].scope.dict->find(id_cache.get("a_field"));
    ASSERT_FALSE(a_field_idx == UndefinedAstNodeIndex);
    ASSERT_STREQ(id_cache.get(ast[a_field_idx].struct_field.name).str, "a_field");
  }
}

TEST(OrderedDict, Simple) {
  OrderedDict<IdIndex, AstNodeIndex, IdIndexHash> dict;
  IdCache id_cache;
  Ast ast;
  {
    auto a_var_idx = ast.create(AstNode::Kind::LocalVariable);
    auto& a_var_node = ast[a_var_idx];
    a_var_node.local_variable.value.type = ast.create(AstNode::Kind::I32Type);
    a_var_node.local_variable.name = id_cache.get("a", 1);
    dict.append(id_cache.get("a", 1), a_var_idx);
  }
  {
    auto b_var_idx = ast.create(AstNode::Kind::LocalVariable);
    auto& b_var_node = ast[b_var_idx];
    b_var_node.local_variable.value.type = ast.create(AstNode::Kind::I32Type);
    b_var_node.local_variable.name = id_cache.get("b", 1);
    dict.append(id_cache.get("b", 1), b_var_idx);
  }
  auto& nodes = dict.get_nodes();
  ASSERT_EQ(nodes.size(), 2);

  auto& a_node = ast[nodes[0]];
  EXPECT_EQ(a_node.kind, AstNode::Kind::LocalVariable);
  EXPECT_STREQ(id_cache.get(a_node.local_variable.name).str, "a");
  
  auto& b_node = ast[nodes[1]];
  EXPECT_EQ(b_node.kind, AstNode::Kind::LocalVariable);
  EXPECT_STREQ(id_cache.get(b_node.local_variable.name).str, "b");
}

TEST(Ast, FunTypeWithNamedParams) {
  Ast ast;
  IdCache id_cache;
  auto f_idx = ast.create(AstNode::Kind::FunTypeWithNamedParams);
  auto& f_node = ast[f_idx];
  auto& fun_type = f_node.fun_type_with_named_params;
  fun_type.fun_type.add_param_type(ast.create(AstNode::Kind::I32Type));
  fun_type.add_name(id_cache.get("a"));
  fun_type.fun_type.add_param_type(ast.create(AstNode::Kind::U32Type));
  fun_type.add_name(id_cache.get("b"));
  fun_type.fun_type.return_type = ast.create(AstNode::Kind::F32Type);

  {
    auto& node = ast[AstNodeIndex(0)];
    ASSERT_EQ(node.kind, AstNode::Kind::FunTypeWithNamedParams);
    ASSERT_NE(node.fun_type.param_types, nullptr);
    auto& param1 = ast[node.fun_type.param_types->operator[](0)];
    auto& param2 = ast[node.fun_type.param_types->operator[](1)];
    ASSERT_EQ(param1.kind, AstNode::Kind::I32Type);
    ASSERT_EQ(param2.kind, AstNode::Kind::U32Type);
    auto& return_type = ast[node.fun_type.return_type];
    ASSERT_EQ(return_type.kind, AstNode::Kind::F32Type);
  }
}

TEST(Ast, Struct) {
  Ast ast;
  IdCache id_cache;
  auto struc_idx = ast.create(AstNode::Kind::Struct);
  auto& struc_node = ast[struc_idx];
}

TEST(Lexer, Simple) {
  std::istringstream in("fun struct union \"test\"");
  Lexer::Tokens tokens;
  IdCache id_cache;
  Lexer lexer(in, tokens, id_cache);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Fun);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Struct);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Union);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::StringLiteral);
  auto string_token = lexer.last();
  ASSERT_STREQ(id_cache.get(string_token.str).str,"test");
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Eof);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Eof);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Eof);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Eof);
  auto eof1_token = &lexer.next();
  auto eof2_token = &lexer.next();
  ASSERT_EQ(eof1_token, eof2_token);
}

TEST(Ast, FunType) {
  Ast ast;
  IdCache id_cache;
  auto i32_type_idx = ast.create(AstNode::Kind::I32Type);
  auto idx = ast.create(AstNode::Kind::FunType);
  auto& node = ast[idx];
  node.fun_type.name = id_cache.get("funtype", 7);
  node.fun_type.return_type = i32_type_idx;
  node.fun_type.param_types = new std::vector<AstNodeIndex>();
  node.fun_type.param_types->emplace_back(i32_type_idx);
  node.fun_type.param_types->emplace_back(i32_type_idx);
  {
    auto& node = ast[idx];
    EXPECT_STREQ(id_cache.get(node.fun_type.name).str, "funtype");
    auto& return_type_node = ast[node.fun_type.return_type];
    EXPECT_EQ(return_type_node.kind, AstNode::Kind::I32Type);
    ASSERT_TRUE(node.fun_type.param_types != nullptr);
    ASSERT_EQ(node.fun_type.param_types->size(), 2);
    auto& param1_node = ast[(*node.fun_type.param_types)[0]];
    auto& param2_node = ast[(*node.fun_type.param_types)[1]];
    EXPECT_EQ(param1_node.kind, AstNode::Kind::I32Type);
    EXPECT_EQ(param2_node.kind, AstNode::Kind::I32Type);
  }
}

TEST(Ast, Stmt) {
  Ast ast;
  IdCache id_cache;
  auto scope_idx = ast.create(AstNode::Kind::BlockScope);
  auto& scope_node = ast[scope_idx];
  auto block_stmt_idx = ast.create(AstNode::Kind::BlockStmt);
  auto& block_stmt = ast[block_stmt_idx];
  scope_node.block_scope.block_stmt = block_stmt_idx;
  block_stmt.block_stmt.block_scope = scope_idx;

  auto i32_type_idx = ast.create(AstNode::Kind::I32Type);
  auto local_var_idx = ast.create(AstNode::Kind::LocalVariable);
  auto& local_var = ast[local_var_idx];
  local_var.local_variable.name = id_cache.get("a");
  local_var.local_variable.value.type = i32_type_idx;

  auto init_idx = ast.create(AstNode::Kind::I32Literal);
  auto& init_node = ast[init_idx];
  init_node.i32_literal.value.type = i32_type_idx;
  init_node.i32_literal.literal_value = 100;

  auto var_decl_stmt_idx = ast.create(AstNode::Kind::VariableDeclStmt);
  auto& var_decl_stmt = ast[var_decl_stmt_idx];
  var_decl_stmt.variable_decl_stmt.variable = local_var_idx;
  var_decl_stmt.variable_decl_stmt.init_expr = init_idx;

  block_stmt.block_stmt.add_stmt(var_decl_stmt_idx);
}

TEST(Ast, Expession) {
  Ast ast;
  IdCache id_cache;
  auto& expr_node = ast[ast.create(AstNode::Kind::AssignExpr)];

  auto left_idx = ast.create(AstNode::Kind::StringLiteral);
  auto& left_node = ast[left_idx];
  left_node.string_literal.string = id_cache.get("string_test", strlen("string_test"));
  expr_node.assign_expr.left = left_idx;

  auto right_idx = ast.create(AstNode::Kind::StringLiteral);
  auto& right_node = ast[right_idx];
  right_node.string_literal.string = id_cache.get("cos", 3);
  expr_node.assign_expr.right = right_idx;

  {
    auto& left_node = ast[expr_node.assign_expr.left];
    auto& right_node = ast[expr_node.assign_expr.right];

    EXPECT_STREQ(id_cache.get(left_node.string_literal.string).str, "string_test");
    EXPECT_STREQ(id_cache.get(right_node.string_literal.string).str, "cos");
  }
}

TEST(StrongType, Simple) {
  struct IdIndexPhantom {};
  using IdIndex = StrongType<uint32_t, IdIndexPhantom>;

  struct IdIndexHash {
    std::size_t operator()(const IdIndex id) const noexcept {
      return id.get();
    }
  };
  using Set = std::unordered_set<IdIndex, IdIndexHash>;
  Set s;
  s.emplace(12);
  s.emplace(15);
  s.emplace(16);
  EXPECT_EQ(sizeof(IdIndex), sizeof(uint32_t));
  EXPECT_NE(s.find(IdIndex(15)), s.end());
}

TEST(Parser, Simple) {
  Lexer::Tokens tokens;
  std::istringstream in("fun test(i8* str, i32 len) -> i32 { return 10 }");
  IdCache id_cache;
  Lexer lexer(in, tokens, id_cache);
  Ast ast;
  Parser parser(lexer, ast, id_cache);
  parser.parse();
}

TEST(Ir, Simple) {
  using namespace ir;
  Function f(0, 0, 0);
  auto& add = f.add(Instr::add, Type::D, Arg{.local_index=1}, Arg{.local_index=2}, Arg{.local_index=3});
  auto& mov = f.add(Instr::mov, Type::D, Arg{.local_index=1}, Arg{.local_index=2});
  auto& jmp = f.add(Instr::jmp, Type::V, Arg{.node_pointer=&add});

  auto node = f.get_head();
  EXPECT_EQ(node->m_instr, Instr::add);
  node = node->m_next;
  EXPECT_EQ(node->m_instr, Instr::mov);
  node = node->m_next;
  EXPECT_EQ(node->m_instr, Instr::jmp);
  node = node->m_next;
  EXPECT_TRUE(node == nullptr);
}

TEST(Ir, Compact) {
  using namespace ir;
  Function f(0, 0, 0);
  f.add(Instr::add, Type::I, Arg{.local_index = 1}, Arg{.local_index = 1}, Arg{.local_index = 2});
  f.add(Instr::add, Type::I, Arg{.local_index = 1}, Arg{.local_index = 1}, Arg{.local_index = 2});
  auto& third_node = f.add(Instr::add, Type::I, Arg{.local_index = 1}, Arg{.local_index = 1}, Arg{.local_index = 2});
  f.add(Instr::add, Type::I, Arg{.local_index = 1}, Arg{.local_index = 1}, Arg{.local_index = 2});
  f.add(Instr::jmp, Type::V, Arg{.node_pointer = &third_node});

  f.compact();
}

TEST(Ir, Test) {
  using namespace ir;
  IdCache id_cache;
  Function f(
    FunctionBuilder()
      .set_args_size(0)
      .set_consts_size(2)
      .set_locals_size(2));
  f.add_const(Value{.str_value = id_cache.get("test", 4)});
  f.add_const(Value{.i_value = 100});
  f.add(Instr::mov, Type::I, Arg{.local_index = 1});
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
