#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

using adjacency_t = std::set<std::pair<std::string, std::string>>;

adjacency_t read_adjacencies()
{
  auto ret = adjacency_t {};

  for (std::string line; std::getline(std::cin, line);) {
    auto bracket = line.find(")");
    auto fst     = line.substr(0, bracket);
    auto snd     = line.substr(bracket + 1, std::string::npos);

    ret.emplace(fst, snd);
  }

  return ret;
}

void topo_visit(
    std::string const& node, adjacency_t const& adj,
    std::map<std::string, int>& marks, std::vector<std::string>& order)
{
  constexpr int permanent = 1;
  constexpr int temporary = 2;

  if (marks.find(node) != marks.end()) {
    if (marks[node] == permanent) {
      return;
    } else if (marks[node] == temporary) {
      assert(false && "Not a DAG");
    }

    marks[node] = temporary;

    for (auto const& [from, to] : adj) {
      if (from == node) {
        topo_visit(to, adj, marks, order);
      }
    }

    marks[node] = permanent;
    order.push_back(node);
  }
}

std::vector<std::string> topo_sort(adjacency_t const& adj)
{
  auto ret   = std::vector<std::string> {};
  auto marks = std::map<std::string, int> {};

  for (auto const& [fst, snd] : adj) {
    marks[fst] = 0;
    marks[snd] = 0;
  }

  while (true) {
    auto next = std::find_if(marks.begin(), marks.end(), [](auto const& p) {
      return p.second == 0;
    });

    if (next == marks.end()) {
      break;
    }

    topo_visit(next->first, adj, marks, ret);
  }

  std::reverse(ret.begin(), ret.end());
  return ret;
}

std::map<std::string, int> distances(adjacency_t const& adj)
{
  auto order = topo_sort(adj);

  auto ret   = std::map<std::string, int> {};
  ret["COM"] = 0;

  for (auto const& from : order) {
    for (auto const& [f, t] : adj) {
      if (from == f) {
        ret[t] = ret[f] + 1;
      }
    }
  }

  return ret;
}

int main()
{
  auto adj   = read_adjacencies();
  auto dists = distances(adj);

  auto total_dist = std::accumulate(
      dists.begin(), dists.end(), 0,
      [](auto acc, auto const& dist) { return acc + dist.second; });
  std::cout << total_dist << '\n';
}
