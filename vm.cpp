#include "vm.hpp"
#include "ir.hpp"

void Vm::run(IdIndex module_name, IdIndex function_name) {
  const auto module_index = m_context.find_module(module_name);

  if (module_index == module_index.undefined) {
    return;
  }
  auto& module = m_context.get_module(module_index);
  const auto function_index = module.find_function(function_name);

  if (function_index == function_index.undefined) {
    return;
  }
  auto& function = module.get_function(function_index);

  auto& code = function.get_compacted_code();
  auto* ip = code.data();
  std::vector<uint8_t> heap;
  std::vector<uint8_t> stack(1024);
  using namespace ir;

  while (true) {
    switch ((uint16_t)*ip) {
      case instr_type(Instr::add, Type::I): {
         
        break;
      }
      case instr_type(Instr::ret, Type::I): {
        break;
      }
    }
  }
}
