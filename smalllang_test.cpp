#include <gtest/gtest.h>
#include <sstream>

#include "lexer.hpp"
#include "token.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "id_cache.hpp"

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
  auto& struct_node = ast.get_node(struct_idx);
  auto a_field_idx = ast.create(AstNode::Kind::StructField);
  auto& a_field = ast.get_node(a_field_idx);
  a_field.node.struct_field.name = id_cache.get("a_field");
  a_field.node.struct_field.value.type = ast.create(AstNode::Kind::I32Type);
  struct_node.node.struc.scope.add_node(a_field_idx, a_field.node.struct_field.name);
  {
    auto a_field_idx = ast.get_node(struct_idx).node.scope.dict->find(id_cache.get("a_field"));
    ASSERT_FALSE(a_field_idx == UndefinedAstNodeIndex);
    ASSERT_STREQ(id_cache.get(ast.get_node(a_field_idx).node.struct_field.name).str, "a_field");
  }
}

TEST(OrderedDict, Simple) {
  OrderedDict dict;
  IdCache id_cache;
  Ast ast;
  {
    auto a_var_idx = ast.create(AstNode::Kind::LocalVariable);
    auto& a_var_node = ast.get_node(a_var_idx);
    a_var_node.node.local_variable.value.type = ast.create(AstNode::Kind::I32Type);
    a_var_node.node.local_variable.name = id_cache.get("a", 1);
    dict.append(id_cache.get("a", 1), a_var_idx);
  }
  {
    auto b_var_idx = ast.create(AstNode::Kind::LocalVariable);
    auto& b_var_node = ast.get_node(b_var_idx);
    b_var_node.node.local_variable.value.type = ast.create(AstNode::Kind::I32Type);
    b_var_node.node.local_variable.name = id_cache.get("b", 1);
    dict.append(id_cache.get("b", 1), b_var_idx);
  }
  auto& nodes = dict.get_nodes();
  ASSERT_EQ(nodes.size(), 2);

  auto& a_node = ast.get_node(nodes[0]);
  EXPECT_EQ(a_node.kind, AstNode::Kind::LocalVariable);
  EXPECT_STREQ(id_cache.get(a_node.node.local_variable.name).str, "a");
  
  auto& b_node = ast.get_node(nodes[1]);
  EXPECT_EQ(b_node.kind, AstNode::Kind::LocalVariable);
  EXPECT_STREQ(id_cache.get(b_node.node.local_variable.name).str, "b");
}

TEST(Ast, FunTypeWithNamedParams) {
  Ast ast;
  IdCache id_cache;
  auto f_idx = ast.create(AstNode::Kind::FunTypeWithNamedParams);
  auto& f_node = ast.get_node(f_idx);
  auto& fun_type = f_node.node.fun_type_with_named_params;
  fun_type.fun_type.add_param_type(ast.create(AstNode::Kind::I32Type));
  fun_type.add_name(id_cache.get("a"));
  fun_type.fun_type.add_param_type(ast.create(AstNode::Kind::U32Type));
  fun_type.add_name(id_cache.get("b"));
  fun_type.fun_type.return_type = ast.create(AstNode::Kind::F32Type);

  {
    auto& node = ast.get_node(0);
    ASSERT_EQ(node.kind, AstNode::Kind::FunTypeWithNamedParams);
    ASSERT_NE(node.node.fun_type.param_types, nullptr);
    auto& param1 = ast.get_node(node.node.fun_type.param_types->operator[](0));
    auto& param2 = ast.get_node(node.node.fun_type.param_types->operator[](1));
    ASSERT_EQ(param1.kind, AstNode::Kind::I32Type);
    ASSERT_EQ(param2.kind, AstNode::Kind::U32Type);
    auto& return_type = ast.get_node(node.node.fun_type.return_type);
    ASSERT_EQ(return_type.kind, AstNode::Kind::F32Type);
  }
}

TEST(Ast, Struct) {
  Ast ast;
  IdCache id_cache;
  auto struc_idx = ast.create(AstNode::Kind::Struct);
  auto& struc_node = ast.get_node(struc_idx);
}

TEST(Lexer, Simple) {
  std::istringstream in("fun struct union \"test\"");
  Lexer::Tokens tokens;
  Lexer lexer(in, tokens);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Fun);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Struct);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::Union);
  ASSERT_EQ(lexer.next().get_kind(), Token::Kind::StringLiteral);
  auto string_token = static_cast<const LiteralToken<std::string, Token::Kind::StringLiteral>*>(&lexer.last());
  ASSERT_STREQ(string_token->get_value().c_str(),"test");
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
  auto& node = ast.get_node(idx);
  node.node.fun_type.name = id_cache.get("funtype", 7);
  node.node.fun_type.return_type = i32_type_idx;
  node.node.fun_type.param_types = new std::vector<AstNodeIndex>();
  node.node.fun_type.param_types->emplace_back(i32_type_idx);
  node.node.fun_type.param_types->emplace_back(i32_type_idx);
  {
    auto& node = ast.get_node(idx);
    EXPECT_STREQ(id_cache.get(node.node.fun_type.name).str, "funtype");
    auto& return_type_node = ast.get_node(node.node.fun_type.return_type);
    EXPECT_EQ(return_type_node.kind, AstNode::Kind::I32Type);
    ASSERT_TRUE(node.node.fun_type.param_types != nullptr);
    ASSERT_EQ(node.node.fun_type.param_types->size(), 2);
    auto& param1_node = ast.get_node((*node.node.fun_type.param_types)[0]);
    auto& param2_node = ast.get_node((*node.node.fun_type.param_types)[1]);
    EXPECT_EQ(param1_node.kind, AstNode::Kind::I32Type);
    EXPECT_EQ(param2_node.kind, AstNode::Kind::I32Type);
  }
}

TEST(Ast, Expession) {
  Ast ast;
  IdCache id_cache;
  auto& expr_node = ast.get_node(ast.create(AstNode::Kind::Expression));

  auto left_idx = ast.create(AstNode::Kind::StringLiteral);
  auto& left_node = ast.get_node(left_idx);
  left_node.node.string_literal.string = id_cache.get("string_test", strlen("string_test"));
  expr_node.node.expression.left = left_idx;

  auto right_idx = ast.create(AstNode::Kind::StringLiteral);
  auto& right_node = ast.get_node(right_idx);
  right_node.node.string_literal.string = id_cache.get("cos", 3);
  expr_node.node.expression.right = right_idx;

  {
    auto& left_node = ast.get_node(expr_node.node.expression.left);
    auto& right_node = ast.get_node(expr_node.node.expression.right);

    EXPECT_STREQ(id_cache.get(left_node.node.string_literal.string).str, "string_test");
    EXPECT_STREQ(id_cache.get(right_node.node.string_literal.string).str, "cos");
  }
}

TEST(Parser, Simple) {
  Lexer::Tokens tokens;
  std::istringstream in("fun test(i8* str, i32 len) -> i32 { return 10 }");
  Lexer lexer(in, tokens);
  Parser parser(lexer);
  parser.parse();
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
