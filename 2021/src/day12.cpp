#include <utils/utils.h>

#include <iostream>
#include <map>
#include <set>

class graph {
public:
  static graph load()
  {
    auto ret = graph();

    utils::for_each_line([&ret](auto const& line) {
      auto parts = utils::split(line, "-");

      ret.adj_.try_emplace(parts[0], std::set<std::string> {});
      ret.adj_.try_emplace(parts[1], std::set<std::string> {});

      ret.adj_.at(parts[0]).insert(parts[1]);
      ret.adj_.at(parts[1]).insert(parts[0]);
    });

    return ret;
  }

  int paths(
      std::string const& start, std::string const& end,
      std::map<std::string, int> visited) const
  {
    if (start == end) {
      return 1;
    }

    if (std::islower(start[0])) {
      visited.try_emplace(start, 0);
      visited.at(start)++;
    }

    auto total = 0;

    for (auto const& next : adj_.at(start)) {
      if (visited.find(next) == visited.end() || visited.at(next) < 1) {
        total += paths(next, end, visited);
      }
    }

    return total;
  }

  int paths_2(
      std::string const& start, std::string const& end,
      std::map<std::string, int> visited) const
  {
    if (start == end) {
      return 1;
    }

    if (std::islower(start[0])) {
      visited.try_emplace(start, 0);
      visited.at(start)++;
    }

    auto total = 0;

    for (auto const& next : adj_.at(start)) {
      auto max = std::all_of(
                     visited.begin(), visited.end(),
                     [](auto const& p) { return p.second < 2; })
                     ? 2
                     : 1;

      if (visited.find(next) == visited.end() || visited.at(next) < max) {
        total += paths(next, end, visited);
      }
    }

    return total;
  }

private:
  graph() = default;

  std::map<std::string, std::set<std::string>> adj_ = {};
};

int part_1(graph const& g) { return g.paths("start", "end", {}); }
int part_2(graph const& g) { return g.paths_2("start", "end", {}); }

int main()
{
  auto data = graph::load();

  std::cout << part_1(data) << '\n';
  std::cout << part_2(data) << '\n';
}
