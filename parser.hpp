#ifndef PARSER_HPP
#define PARSER_HPP

#include "id_cache.hpp"
class Lexer;
class Ast;
class IdCache;

class Parser {
public:
  Parser(Lexer& lexer, Ast& ast, IdCache& id_cache) : m_lexer(lexer), m_ast(ast), m_id_cache(id_cache) {}

  void parse();
private:
  Lexer& m_lexer;
  Ast& m_ast;
  IdCache& m_id_cache;
};

#endif  // PARSER_HPP
