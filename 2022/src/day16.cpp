#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <map>
#include <numeric>
#include <set>

using namespace ctre::literals;

constexpr auto line_pattern
    = "Valve (.*) has flow rate=(\\d+); tunnels? leads? to valves? (.*)"_ctre;

class game {

public:
  static game load(int t)
  {
    auto ret = game(t);

    utils::for_each_line([&ret](auto const& line) {
      auto m = line_pattern.match(line);

      auto name  = m.template get<1>().to_string();
      auto flow  = utils::svtoi(m.template get<2>());
      auto dests = utils::split(m.template get<3>().to_string(), ", ");

      ret.flows_[name] = flow;
      ret.adj_[name]   = std::set(dests.begin(), dests.end());
    });

    return ret;
  }

  void dump()
  {
    fmt::print("digraph G {{\n");
    for (auto [k, v] : flows_) {
      fmt::print("  {0} [label=\"{0}:{1}\"];\n", k, v);
    }
    for (auto [k, v] : adj_) {
      for (auto n : v) {
        fmt::print("  {0} -> {1};\n", k, n);
      }
    }
    fmt::print("}}\n");
  }

private:
  game(int t)
      : time_(t)
  {
  }

  int                                          time_;
  std::map<std::string, int>                   flows_ = {};
  std::map<std::string, std::set<std::string>> adj_   = {};
};

int main()
{
  auto g = game::load(30);
  g.dump();
}
