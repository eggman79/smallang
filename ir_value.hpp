#ifndef IR_VALUE_HPP
#define IR_VALUE_HPP

#include <cstdint>
#include <string>
#include "ordered_dict.hpp"

using IrIndex = uint32_t;
using IrValues = std::vector<IrIndex>;

struct IrObject {
   
};

struct IrFunction : IrObject {
};

struct IrString : IrObject {
  std::string value;
};  

struct IrValue {
  union {
    int64_t i64_value;
    double d_value;
    
  };
  enum class Kind: uint8_t {
    Number, RealNumber, Object,
  };
  Kind kind;
};

struct IrModule {
   
  IrValues ir_values;
};

#endif  // IR_VALUE_HPP
