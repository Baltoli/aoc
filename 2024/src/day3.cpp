#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <ranges>

using namespace ctre::literals;

constexpr auto mul_pattern
    = R"((?<mul>mul\((?<x>\d{1,3}),(?<y>\d{1,3})\))|(?<do>do\(\))|(?<dont>don't\(\)))"_ctre;

static_assert(mul_pattern.match("mul(1,2)"));
static_assert(mul_pattern.match("mul(44,46)"));
static_assert(mul_pattern.match("mul(123,4)"));
static_assert(!mul_pattern.match("mul(4*"));
static_assert(!mul_pattern.match("mul(6,9!"));
static_assert(!mul_pattern.match("?(12,34)"));
static_assert(!mul_pattern.match("mul ( 2 , 4 )"));

static_assert(mul_pattern.match("do()"));
static_assert(mul_pattern.match("don't()"));

template <typename Result>
constexpr long eval(Result r)
{
  return utils::svtol(r.template get<"x">())
         * utils::svtol(r.template get<"y">());
}

long run(std::vector<std::string> const& lines, bool always)
{
  auto sum     = 0L;
  auto enabled = true;

  for (auto const& line : lines) {
    for (auto m : mul_pattern.range(line)) {
      if (m.template get<"mul">() && (enabled || always)) {
        sum += eval(m);
      }

      if (m.template get<"do">()) {
        enabled = true;
      }

      if (m.template get<"dont">()) {
        enabled = false;
      }
    }
  }

  return sum;
}

int main()
{
  auto in = utils::get_lines();
  fmt::print("{}\n", run(in, true));
  fmt::print("{}\n", run(in, false));
}