#pragma once

#include <algorithm>
#include <functional>
#include <generator>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace utils {

std::vector<std::string>
split(std::string const& str, std::string const& delim);

template <typename T>
std::set<T> intersect(std::set<T> const& a, std::set<T> const& b)
{
  auto ret = std::set<T> {};
  auto it  = ret.begin();
  std::set_intersection(
      a.begin(), a.end(), b.begin(), b.end(), std::inserter(ret, it));
  return ret;
}

template <typename T>
std::set<T> set_union(std::set<T> const& a, std::set<T> const& b)
{
  auto ret = std::set<T> {};
  auto it  = ret.begin();
  std::set_union(
      a.begin(), a.end(), b.begin(), b.end(), std::inserter(ret, it));
  return ret;
}

template <typename T>
std::vector<std::vector<T>> group(std::vector<T> const& xs, size_t n)
{
  auto ret = std::vector<std::vector<T>> {};

  for (auto i = 0u; i < xs.size(); i += n) {
    auto next = std::vector<T> {};
    for (auto j = 0u; j < n; ++j) {
      next.push_back(xs[i + j]);
    }
    ret.push_back(next);
  }

  return ret;
}

template <typename It, typename Func, typename Cmp = std::less<>>
auto minimum(It begin, It end, Func&& f)
    -> std::optional<typename std::iterator_traits<It>::value_type>
{
  if (begin == end) {
    return std::nullopt;
  }

  auto min_elt
      = std::min_element(begin, end, [&f](auto const& a, auto const& b) {
          return Cmp {}(std::forward<Func>(f)(a), std::forward<Func>(f)(b));
        });

  return std::forward<Func>(f)(*min_elt);
}

template <typename It, typename Func, typename Cmp = std::less<>>
auto maximum(It begin, It end, Func&& f)
    -> std::optional<typename std::iterator_traits<It>::value_type>
{
  if (begin == end) {
    return std::nullopt;
  }

  auto max_elt
      = std::max_element(begin, end, [&f](auto const& a, auto const& b) {
          return Cmp {}(std::forward<Func>(f)(a), std::forward<Func>(f)(b));
        });

  return std::forward<Func>(f)(*max_elt);
}

template <typename It, typename Func>
long sum(It begin, It end, Func&& f)
{
  return std::accumulate(begin, end, 0L, [&](auto acc, auto const& elt) {
    return acc + std::forward<Func>(f)(elt);
  });
}

std::generator<std::string> lines();

template <typename Func>
void for_each_line(Func&& f)
{
  for (auto line : lines()) {
    std::forward<Func>(f)(line);
  }
}

template <typename T, typename Func>
constexpr auto map(std::vector<T> const& vec, Func&& f) noexcept
    -> std::vector<std::invoke_result_t<Func, T const&>>
{
  using result = std::invoke_result_t<Func, T const&>;

  auto ret = std::vector<result> {};
  std::transform(vec.begin(), vec.end(), std::back_inserter(ret), f);
  return ret;
}

template <typename Func>
auto map_lines(Func&& f)
    -> std::vector<std::invoke_result_t<Func, std::string const&>>
{
  using result = std::invoke_result_t<Func, std::string const&>;
  auto ret     = std::vector<result> {};
  for_each_line([&ret, &f](auto const& line) {
    ret.push_back(std::invoke(std::forward<Func>(f), line));
  });
  return ret;
}

template <typename T>
auto construct_lines() -> std::vector<T>
{
  return map_lines([](auto const& line) { return T(line); });
}

template <typename Func>
auto zip_transform(Func&& f)
{
  return std::ranges::views::transform([&f](auto&& t) {
    return std::apply(std::forward<Func>(f), std::forward<decltype(t)>(t));
  });
}

template <std::ranges::range Range>
bool all_of(Range&& r)
{
  return !std::ranges::contains(std::forward<Range>(r), false);
}

constexpr auto remove_at_index(std::size_t idx)
{
  namespace rv = std::ranges::views;
  return rv::enumerate
         | rv::filter([idx](auto pair) { return std::get<0>(pair) != idx; })
         | rv::transform([](auto pair) { return std::get<1>(pair); });
}

template <typename Range>
auto count(Range&& r)
{
  return std::ranges::fold_left(
      std::forward<Range>(r), 0L,
      [](auto acc, auto const&) { return acc + 1; });
}

std::vector<std::string> get_lines();
std::string              get_single_line();

std::string md5_sum(std::string const& input);

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overload(Ts...) -> overload<Ts...>;

template <typename Int>
  requires(std::is_integral_v<Int>)
constexpr Int to_int(std::string_view sv)
{
  if (!sv.empty() && sv[0] == '+') {
    sv = sv.substr(1);
  }

  Int result;
  std::from_chars(sv.data(), sv.data() + sv.size(), result);
  return result;
}

constexpr long svtol(std::string_view sv) { return to_int<long>(sv); }

int  svtoi(std::string_view);
long stol(std::string const&);
int  stoi(std::string const&);

int  ctoi(char);
long ctol(char);
char itoc(int);
char ltoc(long);

template <typename T>
int sgn(T val)
{
  return (T(0) < val) - (val < T(0));
}

constexpr long pow(long x, long n)
{
  if (n == 0) {
    return 1;
  } else {
    return x * pow(x, n - 1);
  }
}

template <typename Int>
constexpr std::size_t digits(Int i)
  requires(std::is_integral_v<Int>)
{
  if constexpr (std::is_signed_v<Int>) {
    if (i < 0) {
      i = -i;
    }
  }

  auto digits = std::size_t {1};
  while (i >= 10) {
    i /= 10;
    digits += 1;
  }
  return digits;
}

static_assert(digits(0) == 1);
static_assert(digits(1) == 1);
static_assert(digits(9) == 1);
static_assert(digits(10) == 2);
static_assert(digits(99) == 2);
static_assert(digits(-100298) == 6);

inline void hash_combine(std::size_t& seed) { }

template <typename T, typename... Rest>
void hash_combine(std::size_t& seed, T const& v, Rest const&... rest)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  hash_combine(seed, rest...);
}

inline auto hash_combine_curry(std::size_t& seed)
{
  return [&seed](auto const&... args) { hash_combine(seed, args...); };
}

} // namespace utils
