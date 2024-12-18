#include "token.hpp"

const Token::Keywords Token::s_keywords = {
  {"struct", Token::Kind::Struct}, 
  {"class", Token::Kind::Class}, 
  {"var", Token::Kind::Var}, 
  {"val", Token::Kind::Val}, 
  {"union", Token::Kind::Union}, 
  {"fun", Token::Kind::Fun}, 
  {"return", Token::Kind::Return}, 
  {"i32", Token::Kind::I32},
  {"i16", Token::Kind::I16},
  {"i8", Token::Kind::I8},
  {"u32", Token::Kind::U32},
  {"u16", Token::Kind::U16},
  {"u8", Token::Kind::U8},
  {"f32", Token::Kind::F32},
  {"f64", Token::Kind::F64},
};
