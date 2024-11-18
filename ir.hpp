#ifndef IR_HPP
#define IR_HPP

#include <cassert>
#include <cstdint>
#include <vector>
#include <array>
#include "strong_type.hpp"
#include "id_index.hpp"

namespace ir {

struct NodePhantom {};
using NodeIndex = StrongType<std::uint16_t, NodePhantom>;

enum class Type: uint8_t {
  I,
  L,
  D,
  S,
  V,
};
enum class Instr: std::uint8_t {
  mov, add, sub, div, mul, shr, shl, inc, dec, 
  jmp, jg, jl, jge, jle, je, jne, jz, jnz,
  call, callv, ret, retv, label,
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
      return 1;
    case Instr::callv:
    case Instr::retv:
      return 0;
    default:
      assert(false);
  }
}

struct Node {
  union {
    struct {
      Node* m_prev = nullptr;
      Node* m_next = nullptr;
    };
    uint64_t m_offset_during_compacting;
  };
  Instr m_instr;
  Type m_type; 
};

struct Function;

struct Value {
  union {
    IdIndex str_value;
    uint32_t i_value;
    uint64_t l_value;
    IdIndex d_value;
  };
  Type type;
};

struct Arg {
  union {
    uint16_t local_index;
    Node* node_pointer;
    Function* function_pointer;
  };
};

static_assert(sizeof(Arg) == sizeof(uintptr_t), "too big size");

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

  template <typename T>
  void compact_bytes(T bytes) {
    uint8_t* b = (uint8_t*)&bytes;
    for (size_t i = 0; i < sizeof(bytes); ++i) {
      m_compacted_code.emplace_back(b[i]);
    }
  }
public:
  void verify() {

  }

  void compact() {
    assert(!m_compacted && m_compacted_code.empty());
    auto node = get_head();
    m_compacted_code.reserve(1024);
    std::vector<std::pair<Node*, uint64_t>> jump_offsets; 

    while (node) {
      auto next = node->m_next;


      const auto offset = m_compacted_code.size();
      compact_bytes(node->m_instr);
      compact_bytes(node->m_type);
      const auto args_count = instr_to_args_count(node->m_instr, node->m_type);

      switch (node->m_instr) {
        case Instr::ret:
        case Instr::inc:
        case Instr::dec: {
          NodeArgs<1>* node_args = (NodeArgs<1>*)node;
          compact_bytes(node_args->args[0].local_index);
          break;
        }
        case Instr::mov: {
          NodeArgs<2>* node_args = (NodeArgs<2>*)node;
          compact_bytes(node_args->args[0].local_index);
          compact_bytes(node_args->args[1].local_index);
          break;
        }
        case Instr::add:        
        case Instr::sub:        
        case Instr::mul:        
        case Instr::div:        
        case Instr::shl:        
        case Instr::shr: {
          NodeArgs<3>* node_args = (NodeArgs<3>*)node;
          compact_bytes(node_args->args[0].local_index);
          compact_bytes(node_args->args[1].local_index);
          compact_bytes(node_args->args[2].local_index);
          break;
        }
        case Instr::call: {
          NodeArgs<1>* node_args = (NodeArgs<1>*)node;
          break;
        }
        case Instr::je:
        case Instr::jne:
        case Instr::jz:
        case Instr::jnz:
        case Instr::jg:
        case Instr::jge:
        case Instr::jl:
        case Instr::jle:
        case ir::Instr::jmp: {
          auto node_args = (NodeArgs<1>*)node;
          jump_offsets.emplace_back(node_args->args[0].node_pointer, m_compacted_code.size());
          compact_bytes((uint16_t)0);

          switch (node->m_instr) {
            case Instr::jz:
            case Instr::jnz:
              compact_bytes(node_args->args[1].local_index);
              break;
            case Instr::jmp:
              break;
            default:
              compact_bytes(node_args->args[1].local_index);
              compact_bytes(node_args->args[2].local_index);
              break;
          }
          break;
        }
        default:
          assert(false);
          break;
      }     
      node->m_offset_during_compacting = offset;
      node = next;
    }

    for (auto& p : jump_offsets) {
      auto ptr = (uint16_t*)&m_compacted_code.data()[p.second];
      *ptr = (uint16_t)p.first->m_offset_during_compacting;
    }
    m_compacted = true;
    m_compacted_code.shrink_to_fit();
  }
  inline Node& add(Instr instr, Type type) {
    return add_impl<Node>(instr, type);
  }

  template <typename...Args>
  inline Node& add(Instr instr, Type type, Args&&...args) {
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

  Function(uint16_t args_size, int16_t locals_size, uint16_t consts_size) : 
    m_args_size(args_size), m_locals_size(locals_size), m_consts_size(consts_size) {}

  ~Function() {
    auto node = m_head;
    while (node) {
      auto next = node->m_next;
      delete node;
      node = next;
    }
  }


  std::vector<uint8_t>& get_compacted_code() { assert(m_compacted); return m_compacted_code; }
  const std::vector<uint8_t>& get_compacted_code() const { assert(m_compacted); return m_compacted_code; }

  using Consts = std::vector<Value>;

  void set_consts(Consts&& values) {
    m_consts = std::move(values);
  }

  Consts& get_consts() { return m_consts; }
  const Consts& get_consts() const { return m_consts; }

private:
  Node* m_head = nullptr;
  Node* m_tail = nullptr;
  Node* m_insert_point = nullptr;
  std::vector<uint8_t> m_compacted_code;

  Consts m_consts;

  uint16_t m_args_size;
  uint16_t m_locals_size;
  uint16_t m_consts_size;
  bool m_compacted = false;
};

struct Module {

};

}

#endif  // IR_HPP
