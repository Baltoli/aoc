#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <span>
#include <vector>

using namespace ctre::literals;

constexpr auto line_pattern = R"((?<target>\d+): (?<rest>.*))"_ctre;

constexpr bool valid(std::int64_t target, std::span<std::int64_t const> parts)
{
  if (parts.empty()) {
    return false;
  }

  if (parts.size() == 1 && parts.front() == target) {
    return true;
  }

  auto last   = parts.back();
  auto head   = parts.subspan(0, parts.size() - 1);
  auto result = false;

  if (target % last == 0) {
    result = valid(target / last, head);
  }

  if (target >= last) {
    result = result || valid(target - last, head);
  }

  return result;
}

class equation {
public:
  constexpr equation(std::string_view line)
  {
    auto [_, target, rest] = line_pattern.match(line);

    target_     = utils::to_int<std::int64_t>(target);
    components_ = utils::map(
        utils::split(rest.str(), " "), utils::to_int<std::int64_t>);
  }

  constexpr bool valid() const
  {
    return ::valid(target_, std::span(components_));
  }

  constexpr auto target() const { return target_; }

private:
  std::int64_t              target_;
  std::vector<std::int64_t> components_;
};

std::int64_t part_1(std::vector<equation> const& eqns)
{
  return std::accumulate(
      eqns.begin(), eqns.end(), std::int64_t {0}, [](auto acc, auto const& e) {
        return acc + (e.valid() ? e.target() : 0);
      });
}

int main()
{
  auto eqns = utils::construct_lines<equation>();
  fmt::print("{}\n", part_1(eqns));
}
