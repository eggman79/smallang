#ifndef STRONG_TYPE_HPP
#define STRONG_TYPE_HPP

template <typename Type, typename PhantomParam>
class StrongType {
public:
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
  Type m_value;
};

#endif  // STRONG_TYPE_HPP
