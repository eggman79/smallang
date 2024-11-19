#ifndef STRONG_TYPE_HPP
#define STRONG_TYPE_HPP

#include <cstdint>
#include <limits>
#include <vector>

template <typename Type, typename PhantomParam>
class StrongType {
public:
  using value_type = Type;

  static inline const StrongType<Type, PhantomParam> undefined;

  struct Hash {
    std::size_t operator()(const StrongType<Type, PhantomParam>& type) const noexcept {
      return type.get();
    }
  };

  StrongType() = default;
  explicit StrongType(Type value) : m_value(value) {}
  StrongType(const StrongType&) = default;
  StrongType(StrongType&&) = default;
  StrongType& operator=(const StrongType&) = default;
  StrongType& operator=(StrongType&&) = default;

  Type& get() { return m_value;}
  const Type& get() const { return m_value;}

  bool operator==(const StrongType<Type, PhantomParam>& value) const noexcept {
    return m_value == value.m_value;
  }
  bool operator!=(const StrongType<Type, PhantomParam>& value) const noexcept {
    return !operator==(value);
  }
  bool operator<(const StrongType<Type, PhantomParam>& value) const noexcept {
    return m_value < value.m_value;
  }
  bool operator<=(const StrongType<Type, PhantomParam>& value) const noexcept {
    return m_value <= value.m_value;
  }
  bool operator>(const StrongType<Type, PhantomParam>& value) const noexcept {
    return m_value > value.m_value;
  }
  bool operator>=(const StrongType<Type, PhantomParam>& value) const noexcept {
    return m_value >= value.m_value;
  }
private:
  Type m_value = std::numeric_limits<Type>::max();
};

#endif  // STRONG_TYPE_HPP
