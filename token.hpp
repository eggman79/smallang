#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <unordered_map>
#include <type_traits>

class Token {
public:
  enum class Kind {
    None, Fun, Struct, Union, 
    Id, StringLiteral, I32Literal, 
    LeftParen, RightParen, LeftBrace, RightBrace, 
    Add, Sub, Mul, Div, Assign, Equals, Great, Less, GreatOrEqual, LessOrEqual,
    I32, I16, I8, U32, U16, U8, F32, F64,
    Eof, Semicolon, Unknown};

  Token(Kind kind) : m_kind(kind) {}
  Token(const Token&) = delete;
  Token(Token&&) = delete;
  Token& operator=(const Token&) = delete;
  Token& operator=(Token&&) = delete;

  Token::Kind get_kind() const {
    return m_kind;
  }

  static Token::Kind id_2_token_kind(const std::string& text) {
    auto it = s_keywords.find(text);

    if (it == s_keywords.end())
      return Token::Kind::Id;

    return it->second;
  }
private:
  Kind m_kind;
  using Keywords = std::unordered_map<std::string, Token::Kind>;
  static const Keywords s_keywords;
};

class IdToken : public Token {
public:
  IdToken(const std::string& id) : Token(Token::Kind::Id), m_id(id) {}
  IdToken(const IdToken&) = delete;
  IdToken(IdToken&&) = delete;
  IdToken& operator=(const IdToken&) = delete;
  IdToken& operator=(IdToken&&) = delete;
  const std::string& get_id() const { return m_id;}
private:
  const std::string m_id;
};

template <typename Type, Token::Kind TokenKind>
class LiteralToken : public Token {
public:
  LiteralToken(Type value) : Token(TokenKind), m_value(value) {}
  LiteralToken(const LiteralToken&) = delete;
  LiteralToken(LiteralToken&&) = delete;
  LiteralToken& operator=(const LiteralToken&) = delete;
  LiteralToken& operator=(LiteralToken&&) = delete;
  Type get_value() const { return m_value;}
private:
  Type m_value;
};

#endif  // TOKEN_HPP
