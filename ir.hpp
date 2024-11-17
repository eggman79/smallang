#ifndef IR_HPP
#define IR_HPP

#include <cassert>
#include <cstdint>
#include <vector>
#include <tuple>
#include "strong_type.hpp"

namespace ir {

struct NodePhantom {};
using NodeIndex = StrongType<std::uint16_t, NodePhantom>;

enum class Type: uint8_t {
  I,
  L,
  D,
  B,
  V,
};
enum class Instr: std::uint8_t {
  mov, add, sub, div, mul, shr, shl, inc, dec, 
  jmp, jg, jl, jge, jle, je, jne, jz, jnz,
  call, ret, label,
};

static std::size_t instr_to_args_count(Instr instr, Type type) {
  switch (instr) {
    case Instr::jmp:
    case Instr::inc:
    case Instr::dec:
    case Instr::label:
      return 1;
    case Instr::mov:
      return 2;
    case Instr::add:
    case Instr::sub:
    case Instr::div:
    case Instr::mul:
    case Instr::shl:
    case Instr::shr:
    case Instr::jg:
    case Instr::jl:
    case Instr::jge:
    case Instr::jle:
    case Instr::je:
    case Instr::jne:
    case Instr::jz:
    case Instr::jnz:
      return 3;
    case Instr::ret:
    case Instr::call:
      if (type == Type::V) return 0;
      return 1;
    default:
      assert(false);
  }
}

struct Node {
  Node* m_prev = nullptr;
  Node* m_next = nullptr;
  Instr m_instr;
  Type m_type; 
};

struct Arg {
  union {
    uint64_t l_value;
    uint32_t i_value;
    double d_value;
    Node* node_pointer;
  };
};

template <std::size_t ArgsCount>
struct NodeArgs : Node {
  std::array<Arg, ArgsCount> args;
};

struct Function {
private:
  template <typename NodeType>
  NodeType& add_impl(Instr instr, Type type) {
    auto node = new NodeType();
    node->m_instr = instr;
    node->m_type = type;

    if (m_tail) {
      m_tail->m_next = node;
      node->m_prev = m_tail;
      m_tail = node;
    } else {
      m_head = m_tail = node;
    }
    return *node;
  }  

  void compact() {
    auto node = get_head();
    while (node) {
      auto next = node->m_next;
      m_compacted_code.emplace_back((uint8_t)node->m_instr);
      m_compacted_code.emplace_back((uint8_t)node->m_type);

      const auto args_count = instr_to_args_count(node->m_instr, node->m_type);

      switch (node->m_instr) {
        
      }     

      node = next;
    }
  }
public:
  Node& add(Instr instr, Type type) {
    return add_impl<Node>(instr, type);
  }

  template <typename...Args>
  Node& add(Instr instr, Type type, Args&&...args) {
    constexpr auto args_count = sizeof...(args);
    auto& node = add_impl<NodeArgs<args_count>>(instr, type);
    std::size_t i = 0;
    for (auto& arg : {args...}) {
      node.args[i++] = arg;
    }
    return node;
  }

  void set_insert_point(Node* point) {
    m_insert_point = point;
  }

  Node* get_head() const { return m_head;}
  Node* get_tail() const { return m_tail;}

  Function(uint16_t args, int16_t locals, uint16_t consts) : 
    m_args(args), m_locals(locals), m_consts(consts) {}

  ~Function() {
    auto node = m_head;
    while (node) {
      auto next = node->m_next;
      delete node;
      node = next;
    }
  }
private:
  Node* m_head = nullptr;
  Node* m_tail = nullptr;
  Node* m_insert_point = nullptr;
  std::vector<uint8_t> m_compacted_code;

  uint16_t m_args;
  uint16_t m_locals;
  uint16_t m_consts;
  bool m_compacted = false;
};

struct Module {
  
};

}

#endif  // IR_HPP
