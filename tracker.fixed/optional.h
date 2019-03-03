#pragma once

#include <cstddef>
#include <array>

template< size_t N >
using Space = std::array<std::byte, N>;

template <typename T>
class alignas(alignof(T)) Maybe
{
  Space<sizeof(T)> _impl;
  bool             hasValue;

public:
  
  Maybe() noexcept : _impl(), hasValue(false) {}
  Maybe(std::nullopt_t) noexcept : Maybe() {};

  template<typename... VV>
  explicit Maybe(std::in_place_t, VV&&... args) 
  : _impl()
  , hasValue(true)
  {
    new(_impl.data()) T(std::forward<VV>(args)...);
  }
};
