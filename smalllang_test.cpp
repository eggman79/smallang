#include <gtest/gtest.h>
#include <sstream>

#include "lexer.hpp"
#include "token.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "id_cache.hpp"

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

TEST(Ast, Simple) {
  Ast ast;
  auto ast_id = ast.create(AstNode::Kind::Value);
  auto& ast_node = ast.get_node(ast_id);
  IdCache id_cache;
  ast_node.node.value.value_kind = AstNode::ValueKind::LocalVariable;
  ast_node.node.local_variable.id = id_cache.get("a", 1);

  EXPECT_EQ(ast_node.kind, AstNode::Kind::Value);
  EXPECT_EQ(ast_node.node.value.value_kind, AstNode::ValueKind::LocalVariable);

}

TEST(IdCache, Simple) {
  IdCache id_cache;
  auto index = id_cache.get("test", 4);
  EXPECT_EQ(index, id_cache.get("test", 4));
  EXPECT_STREQ(id_cache.get(index).str, "test");
  EXPECT_EQ(id_cache.get(index).length, 4);
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
