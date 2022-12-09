#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>

struct entry {
  long size;
  bool is_dir;

  bool operator<(entry const& other) const
  {
    return std::tie(size, is_dir) < std::tie(other.size, other.is_dir);
  }

  bool operator==(entry const& other) const
  {
    return std::tie(size, is_dir) == std::tie(other.size, other.is_dir);
  }
};

int main()
{
  auto stack = std::vector<std::string> {};
  auto sizes = std::map<std::string, entry> {};

  utils::for_each_line([&](auto const& line) {
    if (auto m = ctre::match<R"(\$ cd \.\.)">(line)) {
      stack.pop_back();
    } else if (auto m = ctre::match<R"(\$ cd (.+))">(line)) {
      stack.push_back(m.template get<1>().to_string());
    } else if (auto m = ctre::match<R"((\d+) (.+))">(line)) {
      auto size = utils::svtol(m.template get<1>().to_view());
      auto name = m.template get<2>().to_string();

      auto path = std::string {};
      for (auto const& dir : stack) {
        path += dir;
        sizes.try_emplace(path, 0, true);
        sizes[path].size += size;
      }

      path += name;
      sizes[name] = {size, false};
    }
  });

  auto total = std::accumulate(
      sizes.begin(), sizes.end(), 0L, [](auto acc, auto const& pair) {
        auto const& [k, v] = pair;
        if (v.is_dir && v.size <= 100000) {
          return acc + v.size;
        } else {
          return acc;
        }
      });

  std::cout << total << '\n';

  constexpr auto total_space = 70000000L;
  constexpr auto req_space   = 30000000L;
  auto           used_space  = sizes.at("/").size;
  auto           free_space  = total_space - used_space;

  auto min_sat = std::numeric_limits<long>::max();
  for (auto [k, v] : sizes) {
    if (v.is_dir && v.size + free_space > req_space && v.size < min_sat) {
      min_sat = v.size;
    }
  }
  std::cout << min_sat << '\n';
}
