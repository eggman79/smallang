#ifndef ORDERED_DICT_HPP
#define ORDERED_DICT_HPP

#include <vector>
#include <unordered_map>

#include "ast_node_index.hpp"
#include "id_index.hpp"

template <typename Key, typename Value>
class OrderedDict {
public:
  OrderedDict() = default;
  OrderedDict(const OrderedDict&) = delete;
  OrderedDict(OrderedDict&&) = delete;
  OrderedDict& operator=(const OrderedDict&) = delete;
  OrderedDict& operator=(OrderedDict&&) = delete;

  void append(Key name, Value node_index) {
    auto it = m_map.find(name);
    if (it == m_map.end()) {
      m_map.emplace(name, node_index);
      m_nodes.emplace_back(node_index);
    }
  }

  void append(Value node_index) {
    m_nodes.emplace_back(node_index);
  }

  AstNodeIndex find(Key name) const {
    auto it = m_map.find(name);

    if (it == m_map.end()) {
      return UndefinedAstNodeIndex;
    }
    return it->second;
  }

  const std::vector<Value>& get_nodes() const { return m_nodes;}

private:
  using Map = std::unordered_map<Key, Value>;
  Map m_map;
  std::vector<Value> m_nodes;
};

#endif  // ORDERED_DICT_HPP
