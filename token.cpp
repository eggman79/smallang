#include "token.hpp"

const Token::Keywords Token::s_keywords = {
  {"struct", Token::Kind::Struct}, 
  {"union", Token::Kind::Union}, 
  {"fun", Token::Kind::Fun}, 
  {"i32", Token::Kind::I32},
  {"i16", Token::Kind::I16},
  {"i8", Token::Kind::I8},
  {"u32", Token::Kind::U32},
  {"u16", Token::Kind::U16},
  {"u8", Token::Kind::U8},
  {"f32", Token::Kind::F32},
  {"f64", Token::Kind::F64},
};
