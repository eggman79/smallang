#include "parser.hpp"
#include "lexer.hpp"

void Parser::parse() {
  auto& token = m_lexer.next(); 
  if (token.get_kind() != Token::Kind::Fun) 
    return;

  auto& id_token = m_lexer.next();

  if (id_token.get_kind() != Token::Kind::Id)
    return;

  auto& left_paren_token = m_lexer.next();

  if (left_paren_token.get_kind() != Token::Kind::LeftParen)
    return;

  m_lexer.next();

  while (m_lexer.last().get_kind() != Token::Kind::RightBrace 
    && m_lexer.last().get_kind() != Token::Kind::Eof) {
    m_lexer.next();
  }
}
