#include <cctype>
#include <fstream>
#include <iostream>
#include "lexer.hpp"
#include "ast.hpp"

int main(int argc, char* argv[]) {
  std::cout << sizeof(AstNode) << std::endl;
  if (argc <= 1) {
    std::cerr << "no input file" << std::endl;
    return -1;
  }
  std::ifstream in(argv[1]);
  Lexer::Tokens tokens;
  Lexer lexer(in, tokens);

  while (lexer.next().get_kind() != Token::Kind::Eof) {
    std::cout << (int)lexer.last().get_kind() << std::endl;
  }
  return 0;
}
