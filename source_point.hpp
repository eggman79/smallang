#ifndef SOURCE_POINT_HPP
#define SOURCE_POINT_HPP

#include <deque>
#include "id_index.hpp"
#include "id_cache.hpp"

using LineNumber = uint32_t;
using ColumnNumber = uint32_t;

class SourcePoint {
public:
  IdIndex m_file;
  LineNumber m_line;
  ColumnNumber m_col;
};

using SourcePoints = std::deque<SourcePoint>;
using SourcePointIndex = uint32_t;

class SourcePointFactory {
public:
  SourcePointFactory(IdCache& id_cache) : m_id_cache(id_cache) {

  }

  SourcePointIndex make(const char* filename, size_t filename_len, LineNumber line, ColumnNumber col) {
    const auto file_index = m_id_cache.get(filename, filename_len);
    return make(file_index, line, col);
  }

  SourcePointIndex make(IdIndex file_index, LineNumber line, ColumnNumber col) {
    const auto result = m_source_points.size();
    m_source_points.emplace_back();
    auto& sp = m_source_points.back();
    sp.m_file = file_index;
    sp.m_line = line;
    sp.m_col = col;
    return result;
  }

  const SourcePoints& getSourcePoints() const { return m_source_points;}
private:
  IdCache m_id_cache;
  SourcePoints m_source_points;
};

#endif  // SOURCE_POINT_HPP
