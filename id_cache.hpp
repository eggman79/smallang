#ifndef ID_CACHE_HPP
#define ID_CACHE_HPP

#include <cstdint>
#include <cstring>
#include <utility>
#include <vector>
#include <unordered_map>

using IdIndex = uint32_t;

class IdCache {
public:
  struct String {
    const char* str;
    uint32_t length;
  };

  IdIndex get(const char* str, uint32_t length) {
    StringPair sp{str, length};
    auto it = m_map.find(sp);
    if (it == m_map.end()) {
      auto s = new char[length + 1];
      strncpy(s, str, length);
      s[length] = '\0';

      const auto id_index = m_strings.size();
      m_map[sp] = id_index;
      m_strings.emplace_back(String{s, length});
      return id_index;
    }
    return it->second;
  }

  const String& get(IdIndex index) const {
    return m_strings[index];
  }

private:
  using StringPair = std::pair<const char*, uint32_t>;
  struct StringEqual {
    bool operator()(const StringPair& s1, const StringPair& s2) const {
      if (s1.second != s2.second) 
        return false;

      return !strcmp(s1.first, s2.first);
    }
  };

  struct StringHash {
    std::size_t operator()(const StringPair& s) const {
      auto h = std::hash<uint32_t>{}(s.second);
      for (uint32_t i = 0; i < s.second; ++i) {
        auto ch = std::hash<char>{}(s.first[i]);
        h = h ^ (ch << 1);
      }
      return h;
    }
  };

  using Map = std::unordered_map<StringPair, IdIndex, StringHash, StringEqual>;
  Map m_map;
  std::vector<String> m_strings;
};

#endif  // ID_CACHE_HPP
