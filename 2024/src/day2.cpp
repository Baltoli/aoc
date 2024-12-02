#include <utils/utils.h>

#include <fmt/format.h>

#include <algorithm>
#include <ranges>

namespace ranges = std::ranges;
namespace views  = std::ranges::views;

bool is_safe(std::vector<long> const& report)
{
  auto monotonic = ranges::is_sorted(report, std::less {})
                   || ranges::is_sorted(report, std::greater {});

  auto diffs_ok = utils::all_of(
      views::adjacent<2>(report) | utils::zip_transform([](auto a, auto b) {
        auto diff = std::abs(a - b);
        return diff >= 1 && diff <= 3;
      }));

  return monotonic && diffs_ok;
}

std::int64_t part_1(std::vector<std::vector<long>> const& reports)
{
  return ranges::count_if(reports, [](auto const& r) { return is_safe(r); });
}

std::int64_t part_2(std::vector<std::vector<long>> const& reports)
{
  return ranges::count_if(reports, [](auto const& r) {
    for (auto i = 0u; i < r.size(); ++i) {
      if (is_safe(r | utils::remove_at_index(i) | ranges::to<std::vector>())) {
        return true;
      }
    }

    return false;
  });
}

int main()
{
  auto reports = utils::map_lines([](auto const& line) {
    auto entry_strs = utils::split(line, " ");
    return utils::map(entry_strs, utils::stol);
  });

  fmt::print("{}\n", part_1(reports));
  fmt::print("{}\n", part_2(reports));
}
