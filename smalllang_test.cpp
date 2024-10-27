#include <gtest/gtest.h>
#include <sstream>

#include "lexer.hpp"
#include "token.hpp"

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

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
