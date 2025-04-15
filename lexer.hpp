#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <string>
#include <istream>
#include <memory>

#include "token.hpp"
#include "id_cache.hpp"

class Lexer {
public:
  using Tokens = std::vector<Token>;

  Lexer(std::istream& in, Tokens& tokens, IdCache& id_cache) : 
    m_in(in), m_tokens(tokens), m_id_cache(id_cache) {
    m_buffer.reserve(1024);
    next_char();
  }

  const Token& last() {
    return m_tokens.back();
  }

  const Token& next() {
    omit_white_spaces(); 
    m_buffer.clear();

    if (m_last_char == EOF) {
      if (m_tokens.empty() || m_tokens.back().get_kind() != Token::Kind::Eof) {
        m_tokens.emplace_back(Token::Kind::Eof);
      }
    }
    else if (::isalpha(m_last_char) || m_last_char == '_') {
      do {
        m_buffer += m_last_char;
        next_char();
      } while (::isalpha(m_last_char) || ::isdigit(m_last_char) || m_last_char == '_');

      const auto token_kind = Token::id_2_token_kind(m_buffer);
      
      if (token_kind == Token::Kind::Id) {
        m_tokens.emplace_back(token_kind);
        m_tokens.back().id = m_id_cache.get(m_buffer.c_str(), m_buffer.size());
      } else {
        m_tokens.emplace_back(token_kind);
      }
    } else if (::isdigit(m_last_char)) {
      do {
        m_buffer += m_last_char;
        next_char();
      } while (::isdigit(last_char()));
      const auto value = atoi(m_buffer.c_str());
      m_tokens.emplace_back(Token::Kind::I32Literal);
      m_tokens.back().i32 = value;
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
        case ':': push_token_kind(Token::Kind::Colon); break;
        case '=': push_token_kind(Token::Kind::Assign); break;
        case '>': push_token_kind(Token::Kind::Great); break;
        case '<': push_token_kind(Token::Kind::Less); break;
        case '\'': {
          const auto chr = next_char();
          if (next_char() != '\'') {
            //error
          } else {

          }
          break;
        }
        case '"': {
          next_char();
          while (last_char() != '"') {
            m_buffer += last_char();
            next_char();
          } 
          m_tokens.emplace_back(Token::Kind::StringLiteral);
          m_tokens.back().str = m_id_cache.get(m_buffer.c_str(), m_buffer.size());
          break;
        }
        default:
          push_token_kind(Token::Kind::Unknown);
          break;
      }
      next_char();
    }
    return m_tokens.back();
  }

private:
  std::istream& m_in;
  Tokens& m_tokens;
  std::string m_buffer;
  IdCache& m_id_cache;
  char m_last_char = 0;

  inline char next_char() {
    m_last_char = m_in.get() ;
    return m_last_char;
  }

  inline char last_char() {
    return m_last_char;
  }

  inline void push_token_kind(Token::Kind kind) {
    m_tokens.emplace_back(kind);
  }

  inline void omit_white_spaces() {
    while (::isspace(last_char())) {
      next_char();
    }
  }
};

#endif  // LEXER_HPP
