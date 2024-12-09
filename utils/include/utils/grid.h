#pragma once

#include <utils/vector.h>

#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <generator>
#include <limits>
#include <ranges>
#include <vector>

namespace utils {

struct no_pad { };

template <char C>
struct fill_pad {
  constexpr static char value = C;
};

template <typename Pad>
struct is_pad_policy {
  static constexpr bool value = false;
};

template <>
struct is_pad_policy<no_pad> {
  static constexpr bool value = true;
};

template <char C>
struct is_pad_policy<fill_pad<C>> {
  static constexpr bool value = true;
};

template <typename Pad>
constexpr bool is_pad_policy_v = is_pad_policy<Pad>::value;

struct point {
  std::int64_t x;
  std::int64_t y;

  constexpr std::int64_t magnitude() noexcept { return (x * x) + (y * y); }

  constexpr point right_turn() noexcept;
};

constexpr bool operator==(point const& a, point const& b) noexcept
{
  return std::tie(a.x, a.y) == std::tie(b.x, b.y);
}

constexpr point operator+(point const& a, point const& b) noexcept
{
  return point {a.x + b.x, a.y + b.y};
}

constexpr point& operator+=(point& a, point const& b) noexcept
{
  a = a + b;
  return a;
}

constexpr point operator-(point const& a, point const& b) noexcept
{
  return point {a.x - b.x, a.y - b.y};
}

constexpr point& operator-=(point& a, point const& b) noexcept
{
  a = a - b;
  return a;
}

constexpr point operator*(point const& p, std::int64_t s) noexcept
{
  return point {p.x * s, p.y * s};
}

constexpr point& operator*=(point& a, std::int64_t s) noexcept
{
  a = a * s;
  return a;
}

constexpr point point::right_turn() noexcept
{
  assert(magnitude() == 1);

  if (*this == point {-1, 0}) {
    return point {0, -1};
  }

  if (*this == point {0, -1}) {
    return point {1, 0};
  }

  if (*this == point {1, 0}) {
    return point {0, 1};
  }

  if (*this == point {0, 1}) {
    return point {-1, 0};
  }

  assert(false);
}

template <typename Pad = no_pad, typename Element = char>
  requires(is_pad_policy_v<Pad>)
class grid;

template <typename Pad, typename Element>
struct getter {
  static Element at(grid<Pad> const&, point);
};

template <typename Pad, typename Element>
  requires(is_pad_policy_v<Pad>)
class grid {
public:
  using element_t = Element;

  template <typename Range>
  grid(Range&& lines)
      : data_ {}
      , width_ {0}
      , height_ {0}
  {
    for (auto&& line : std::forward<Range>(lines)) {
      assert(width_ == 0 || width_ == line.size());
      width_ = line.size();

      std::ranges::copy(line, std::back_inserter(data_));
      height_ += 1;
    }

    assert(width_ <= std::numeric_limits<std::int64_t>::max());
    assert(height_ <= std::numeric_limits<std::int64_t>::max());
  }

  std::generator<point> coords() const
  {
    for (auto x = 0L; x < width_; ++x) {
      for (auto y = 0L; y < height_; ++y) {
        co_yield point {x, y};
      }
    }
  }

  std::generator<point> neighbour_deltas() const
  {
    for (auto dx = -1; dx <= 1; ++dx) {
      for (auto dy = -1; dy <= 1; ++dy) {
        if (dx != 0 || dy != 0) {
          co_yield point {dx, dy};
        }
      }
    }
  }

  Element at(point p) const { return getter<Pad, Element>::at(*this, p); }

  void dump() const
  {
    for (auto y = 0; y < height_; ++y) {
      for (auto x = 0; x < width_; ++x) {
        fmt::print(stderr, "{}", at({x, y}));
      }
      fmt::print(stderr, "\n");
    }
  }

  template <typename, typename>
  friend struct getter;

protected:
  template <typename Self>
  auto&& at_unchecked(this Self&& self, point p)
  {
    return self.data_[p.y * self.width_ + p.x];
  }

  bool in_bounds(point p) const
  {
    return p.x >= 0 && p.x < width_ && p.y >= 0 && p.y < height_;
  }

  std::vector<Element> data_;
  std::size_t          width_;
  std::size_t          height_;
};

template <typename Element>
struct getter<no_pad, Element> {
  static Element at(grid<no_pad> const& g, point p)
  {
    if (!g.in_bounds(p)) {
      throw std::invalid_argument("out of bounds");
    }

    return g.at_unchecked(p);
  }
};

template <char C, typename Element>
struct getter<fill_pad<C>, Element> {
  static Element at(grid<fill_pad<C>> const& g, point p)
  {
    if (!g.in_bounds(p)) {
      return C;
    }

    return g.at_unchecked(p);
  }
};

} // namespace utils

namespace std {
template <>
struct hash<::utils::point> {
  size_t operator()(::utils::point const&) const;
};

template <>
struct hash<std::pair<::utils::point, ::utils::point>> {
  size_t operator()(std::pair<::utils::point, ::utils::point> const& p) const;
};
} // namespace std
