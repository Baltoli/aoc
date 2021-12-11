#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace utils {

std::vector<std::string>
split(std::string const& str, std::string const& delim);

template <typename Func>
void for_each_line(Func&& f)
{
  for (std::string line; std::getline(std::cin, line);) {
    std::forward<Func>(f)(line);
  }
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

std::vector<std::string> get_lines();

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

inline void hash_combine(std::size_t& seed) { }

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest)
{
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  hash_combine(seed, rest...);
}

} // namespace utils
