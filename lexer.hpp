#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <istream>
#include <memory>

#include "token.hpp"

class Lexer {
public:
  using Tokens = std::vector<std::unique_ptr<Token>>;

  Lexer(std::istream& in, Tokens& tokens) : m_in(in), m_tokens(tokens) {
    m_buffer.reserve(1024);
    next_char();
  }

  const Token& last() {
    return *m_tokens.back();
  }

  const Token& next() {
    omit_white_spaces(); 
    m_buffer.clear();

    if (m_last_char == EOF) {
      if (m_tokens.empty() || m_tokens.back()->get_kind() != Token::Kind::Eof) {
        m_tokens.emplace_back(std::make_unique<Token>(Token::Kind::Eof));
      }
    }
    else if (::isalpha(m_last_char) || m_last_char == '_') {
      do {
        m_buffer += m_last_char;
        next_char();
      } while (::isalpha(m_last_char) || ::isdigit(m_last_char) || m_last_char == '_');

      const auto token_kind = Token::id_2_token_kind(m_buffer);
      
      if (token_kind == Token::Kind::Id) {
        m_tokens.emplace_back(std::make_unique<IdToken>(m_buffer));
      } else {
        m_tokens.emplace_back(std::make_unique<Token>(token_kind));
      }
    } else if (::isdigit(m_last_char)) {
      do {
        m_buffer += m_last_char;
        next_char();
      } while (::isdigit(last_char()));
      const auto value = atoi(m_buffer.c_str());
      m_tokens.emplace_back(std::make_unique<LiteralToken<int32_t, Token::Kind::I32Literal>>(value));
    } else {
      switch (m_last_char) {
        case '(': push_token_kind(Token::Kind::LeftParen); break;
        case ')': push_token_kind(Token::Kind::RightParen); break;
        case '{': push_token_kind(Token::Kind::LeftBrace); break;
        case '}': push_token_kind(Token::Kind::RightBrace); break;
        case '+': push_token_kind(Token::Kind::Add); break;
        case '-': push_token_kind(Token::Kind::Sub); break;
        case '*': push_token_kind(Token::Kind::Mul); break;
        case '/': push_token_kind(Token::Kind::Div); break;
        case ';': push_token_kind(Token::Kind::Semicolon); break;
        case '=': push_token_kind(Token::Kind::Assign); break;
        case '>': push_token_kind(Token::Kind::Great); break;
        case '<': push_token_kind(Token::Kind::Less); break;
        case '"': {
          next_char();
          while (last_char() != '"') {
            m_buffer += last_char();
            next_char();
          } 
          m_tokens.emplace_back(std::make_unique<LiteralToken<std::string, Token::Kind::StringLiteral>>(m_buffer));
          break;
        }
        default:
          push_token_kind(Token::Kind::Unknown);
          break;
      }
      next_char();
    }
    return *m_tokens.back();
  }

private:
  std::istream& m_in;
  Tokens& m_tokens;
  std::string m_buffer;
  char m_last_char = 0;

  inline char next_char() {
    m_last_char = m_in.get() ;
    return m_last_char;
  }

  inline char last_char() {
    return m_last_char;
  }

  inline void push_token_kind(Token::Kind kind) {
    m_tokens.emplace_back(std::make_unique<Token>(kind));
  }

  void omit_white_spaces() {
    while (::isspace(last_char())) {
      next_char();
    }
  }
};

#endif  // LEXER_HPP
