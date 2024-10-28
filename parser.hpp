#ifndef PARSER_HPP
#define PARSER_HPP

class Lexer;

class Parser {
public:
  Parser(Lexer& lexer) : m_lexer(lexer) {}

  void parse();
private:
  Lexer& m_lexer;
};

#endif  // PARSER_HPP
