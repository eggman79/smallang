#ifndef VM_HPP
#define VM_HPP

#include "id_index.hpp"
#include "ir.hpp"

class Vm {
public:
  Vm(ir::Context& context) : m_context(context) {}

  void run(IdIndex module_name, IdIndex function_name);
private:
  ir::Context& m_context;
};

#endif  // VM_HPP
