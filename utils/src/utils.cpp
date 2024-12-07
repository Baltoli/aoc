#include "md5.h"

#include <utils/grid.h>
#include <utils/utils.h>

#include <array>
#include <charconv>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace utils {

std::vector<std::string> get_lines()
{
  auto ret = std::vector<std::string> {};
  for_each_line([&ret](auto line) { ret.emplace_back(line); });
  return ret;
}

std::string get_single_line() { return get_lines()[0]; }

std::generator<std::string> lines()
{
  for (std::string line; std::getline(std::cin, line);) {
    co_yield line;
  }
}

std::string md5_sum(std::string const& input)
{
  constexpr auto buf_size = 64;

  MD5_CTX ctx;
  MD5Init(&ctx);

  auto end = input.size();
  for (int i = 0; i < input.size(); i += buf_size) {
    auto len = (i + buf_size >= end) ? (end - i) : buf_size;
    MD5Update(&ctx, (unsigned char*)(input.c_str() + i), len);
  }

  auto ret = std::array<unsigned char, 16> {};
  MD5Final(ret.data(), &ctx);

  std::stringstream ss;
  for (auto i : ret) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)i;
  }

  return ss.str();
}

int svtoi(std::string_view sv)
{
  if (!sv.empty() && sv[0] == '+') {
    sv = sv.substr(1);
  }

  int result;
  std::from_chars(sv.data(), sv.data() + sv.size(), result);
  return result;
}

long stol(std::string const& s) { return std::stol(s); }

int stoi(std::string const& s) { return std::stoi(s); }

int  ctoi(char c) { return c - '0'; }
long ctol(char c) { return c - '0'; }
char itoc(int i) { return i + '0'; }
char ltoc(long l) { return l + '0'; }

} // namespace utils

size_t std::hash<::utils::point>::operator()(::utils::point const& p) const
{
  auto seed = size_t {0};
  utils::hash_combine(seed, p.x);
  utils::hash_combine(seed, p.y);
  return seed;
}