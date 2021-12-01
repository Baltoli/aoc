#include <utils/ctre.h>
#include <utils/utils.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <string_view>

using namespace ctre::literals;

struct result {
  long             value;
  std::string_view rest;
};

long eval(std::string_view sv);
long eval2(std::string_view sv);

result initial_value(std::string_view sv)
{
  if (sv[0] == '(') {
    auto depth   = 1;
    auto begin_p = 0;
    auto end_p   = 1;
    for (; depth > 0; ++end_p) {
      auto c = sv[end_p];
      depth += (c == '(' ? 1 : (c == ')' ? -1 : 0));
    }

    auto inside = sv.substr(1, end_p - begin_p - 2);
    if (end_p == sv.size()) {
      return {eval(inside), ""};
    } else {
      return {eval(inside), sv.substr(end_p + 1)};
    }

  } else if (auto m = "(\\d+) (.*)"_ctre.match(sv)) {
    return {utils::svtol(m.template get<1>()), m.template get<2>()};
  } else if (auto m = "(\\d+)"_ctre.match(sv)) {
    return {utils::svtol(m.template get<1>()), {}};
  } else {
    std::cerr << "@" << sv << "@\n";
    assert(false && "Bad initial value");
  }
}

result initial_value2(std::string_view sv)
{
  if (sv[0] == '(') {
    auto depth   = 1;
    auto begin_p = 0;
    auto end_p   = 1;
    for (; depth > 0; ++end_p) {
      auto c = sv[end_p];
      depth += (c == '(' ? 1 : (c == ')' ? -1 : 0));
    }

    auto inside = sv.substr(1, end_p - begin_p - 2);
    if (end_p == sv.size()) {
      return {eval2(inside), ""};
    } else {
      return {eval2(inside), sv.substr(end_p + 1)};
    }

  } else if (auto m = "(\\d+) (.*)"_ctre.match(sv)) {
    return {utils::svtol(m.template get<1>()), m.template get<2>()};
  } else if (auto m = "(\\d+)"_ctre.match(sv)) {
    return {utils::svtol(m.template get<1>()), {}};
  } else {
    std::cerr << "@" << sv << "@\n";
    assert(false && "Bad initial value");
  }
}

result accumulate(long acc, std::string_view sv)
{
  constexpr auto pattern = "(\\+|\\*) (.*)"_ctre;
  auto           m       = pattern.match(sv);

  auto op         = m.template get<1>().str()[0];
  auto [iv, rest] = initial_value(m.template get<2>().str());

  if (op == '+') {
    return {acc + iv, rest};
  } else if (op == '*') {
    return {acc * iv, rest};
  } else {
    std::cerr << "BAD OP: '" << op << "'\n";
    assert(false);
  }
}

result accumulate2(long acc, std::string_view sv)
{
  constexpr auto pattern = "(\\+|\\*) (.*)"_ctre;
  auto           m       = pattern.match(sv);

  auto             op     = m.template get<1>().str()[0];
  std::string_view remain = m.template get<2>();
  auto [iv, rest]         = initial_value2(remain);

  if (op == '+') {
    return {acc + iv, rest};
  } else if (op == '*') {
    auto rhs = eval2(remain);
    return {acc * rhs, ""};
  } else {
    std::cerr << "BAD OP: '" << op << "'\n";
    assert(false);
  }
}

long eval(std::string_view sv)
{
  auto [iv, rest] = initial_value(sv);

  while (!rest.empty()) {
    auto acc_res = accumulate(iv, rest);
    iv           = acc_res.value;
    rest         = acc_res.rest;
  }

  return iv;
}

long eval2(std::string_view sv)
{
  auto [iv, rest] = initial_value2(sv);

  while (!rest.empty()) {
    auto acc_res = accumulate2(iv, rest);
    iv           = acc_res.value;
    rest         = acc_res.rest;
  }

  return iv;
}

int main()
{
  long sum  = 0;
  long sum2 = 0;

  utils::for_each_line([&](auto const& line) {
    sum += eval(line);
    sum2 += eval2(line);
  });

  std::cout << sum << '\n';
  std::cout << sum2 << '\n';
}
