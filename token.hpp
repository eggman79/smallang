#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <unordered_map>
#include <type_traits>
#include "id_index.hpp"
#include "strong_type.hpp"

class Token {
public:
  enum class Kind {
    None, Fun, Class, Struct, Union, Return, Var, Val,
    Id, StringLiteral, I32Literal, 
    LeftParen, RightParen, LeftBrace, RightBrace, 
    Add, Sub, Mul, Div, Assign, Equals, Great, Less, GreatOrEqual, LessOrEqual,
    I32, I16, I8, U32, U16, U8, F32, F64,
    Eof, Semicolon, Unknown};

  Token(Kind kind) : m_kind(kind) {}
  Token(const Token&) = default;
  Token(Token&&) = default;
  Token& operator=(const Token&) = default;
  Token& operator=(Token&&) = default;

  Token::Kind get_kind() const {
    return m_kind;
  }

  static Token::Kind id_2_token_kind(const std::string& text) {
    auto it = s_keywords.find(text);

    if (it == s_keywords.end())
      return Token::Kind::Id;

    return it->second;
  }
  Kind m_kind;
  union {
    IdIndex id; 
    int32_t i32;
    int16_t i16;
    int8_t i8;
    uint32_t u32;
    uint16_t u16;
    uint8_t u8;
    IdIndex str;
  };
private:
  using Keywords = std::unordered_map<std::string, Token::Kind>;
  static const Keywords s_keywords;
};

struct TokenPhantom {};
using TokenIndex = StrongType<std::uint32_t, TokenPhantom>;

struct TokenIndexHash {
  std::size_t operator()(const TokenIndex index) const noexcept {
    return index.get();
  }
};

struct TokenRange {
  TokenIndex from;
  TokenIndex to;

  uint32_t count() const { return to.get() - from.get() + 1; }
};

#endif  // TOKEN_HPP
