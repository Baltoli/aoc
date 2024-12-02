#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <optional>
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

template <typename Func>
void for_each_line(Func&& f)
{
  for (std::string line; std::getline(std::cin, line);) {
    std::forward<Func>(f)(line);
  }
}

template <typename T, typename Func>
auto map(std::vector<T> const& vec, Func&& f)
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

std::vector<std::string> get_lines();
std::string              get_single_line();

std::string md5_sum(std::string const& input);

template <class... Ts>
struct overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
overload(Ts...) -> overload<Ts...>;

int  svtoi(std::string_view);
long svtol(std::string_view);
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

inline void hash_combine(std::size_t& seed) { }

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  hash_combine(seed, rest...);
}

} // namespace utils
