#pragma once

#include <algorithm>
#include <cassert>
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

template <typename Range, typename Func>
std::int64_t sum(Range&& r, Func&& f)
{
  return std::ranges::fold_left(
      std::forward<Range>(r), std::int64_t {0}, [&](auto acc, auto const& elt) {
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

template <typename Int>
char itoc(Int i)
  requires(std::integral<Int>)
{
  assert(i >= 0 && i <= 9);
  return i + '0';
}

char ltoc(long);

template <typename Int>
  requires(std::integral<Int>)
constexpr Int c_to(char c) noexcept
{
  return static_cast<Int>(c) - static_cast<Int>('0');
}

constexpr int  ctoi(char c) noexcept { return c_to<int>(c); }
constexpr long ctol(char c) noexcept { return c_to<long>(c); }

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

constexpr std::int64_t gcd(std::int64_t a, std::int64_t b)
{
  if (b == 0) {
    return a;
  }

  return gcd(b, a % b);
}

static_assert(gcd(3, 6) == 3);
static_assert(gcd(17, 19) == 1);
static_assert(gcd(32657, 0) == 32657);
static_assert(gcd(1, 328) == 1);

struct egcd_result {
  std::int64_t x;
  std::int64_t y;
  std::int64_t g;
};

constexpr bool operator==(egcd_result const& a, egcd_result const& b)
{
  return std::tie(a.x, a.y, a.g) == std::tie(b.x, b.y, b.g);
}

constexpr egcd_result egcd(std::int64_t a, std::int64_t b)
{
  if (b == 0) {
    return {1, 0, a};
  }

  auto [x_1, y_1, g] = egcd(b, a % b);
  return {y_1, x_1 - y_1 * (a / b), g};
}

static_assert(egcd(237, 32) == egcd_result {5, -37, 1});

constexpr std::int64_t solve_crt(
    std::int64_t a_0, std::int64_t n_0, std::int64_t a_1, std::int64_t n_1)
{
  auto [coef_n_0, coef_n_1, gcd] = egcd(n_0, n_1);
  assert(gcd == 1);

  auto mod    = n_0 * n_1;
  auto result = (a_0 * n_1 * coef_n_1) + (a_1 * n_0 * coef_n_0);
  result %= mod;

  if (result < 0) {
    result += mod;
  }

  return result;
}

static_assert(solve_crt(48, 101, 1, 103) == 7623);

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

template <typename T>
T unwrap(std::optional<T> opt)
{
  if (opt.has_value()) {
    return *opt;
  }

  assert(false && "Bad unwrap!");
}

} // namespace utils

namespace std {
template <typename A, typename B>
struct hash<std::pair<A, B>> {
  size_t operator()(std::pair<A, B> const& p) const
  {
    auto seed = std::size_t {0};
    utils::hash_combine(seed, p.first, p.second);
    return seed;
  }
};
} // namespace std