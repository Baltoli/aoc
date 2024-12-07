#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <optional>
#include <span>
#include <vector>

using namespace ctre::literals;

constexpr auto line_pattern = R"((?<target>\d+): (?<rest>.*))"_ctre;

constexpr std::optional<std::int64_t>
prefix(std::int64_t target, std::int64_t last) noexcept
{
  if (last > target) {
    return std::nullopt;
  }

  auto modulus   = utils::pow(10, utils::digits(last));
  auto remainder = target - last;

  if (remainder % modulus == 0) {
    return remainder / modulus;
  }

  return std::nullopt;
}

constexpr bool valid(
    std::int64_t target, std::span<std::int64_t const> parts,
    bool concat) noexcept
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
    result = valid(target / last, head, concat);
  }

  if (target >= last) {
    result = result || valid(target - last, head, concat);
  }

  if (auto pre = prefix(target, last); pre && concat) {
    result = result || valid(*pre, head, concat);
  }

  return result;
}

class equation {
public:
  constexpr equation(std::string_view line) noexcept
  {
    auto [_, target, rest] = line_pattern.match(line);

    target_     = utils::to_int<std::int64_t>(target);
    components_ = utils::map(
        utils::split(rest.str(), " "), utils::to_int<std::int64_t>);
  }

  constexpr bool valid(bool concat) const noexcept
  {
    return ::valid(target_, std::span(components_), concat);
  }

  constexpr auto target() const noexcept { return target_; }

private:
  std::int64_t              target_;
  std::vector<std::int64_t> components_;
};

template <typename Range>
constexpr std::int64_t run(Range&& eqns, bool concat) noexcept
{
  using std::begin;
  using std::end;

  return std::accumulate(
      begin(std::forward<Range>(eqns)), end(std::forward<Range>(eqns)),
      std::int64_t {0}, [concat](auto acc, auto const& e) {
        return acc + (e.valid(concat) ? e.target() : 0);
      });
}

int main()
{
  auto eqns = utils::construct_lines<equation>();
  fmt::print("{}\n", run(eqns, false));
  fmt::print("{}\n", run(eqns, true));
}
