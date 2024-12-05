#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <algorithm>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

using update = std::vector<long>;

class input {
public:
  static input read()
  {
    auto in            = input {};
    auto reading_rules = true;

    utils::for_each_line([&](auto const& line) {
      if (line.empty()) {
        reading_rules = false;
        return;
      }

      if (reading_rules) {
        auto [_, a_s, b_s] = ctre::match<"(\\d+)\\|(\\d+)">(line);

        auto a = utils::svtol(a_s);
        auto b = utils::svtol(b_s);

        in.edges_.try_emplace(a);
        in.edges_[a].insert(b);
      } else {
        auto update = utils::map(utils::split(line, ","), utils::stol);
        in.updates_.push_back(update);
      }
    });

    return in;
  }

  bool is_sorted(update const& u) const
  {
    auto seen = std::unordered_set<long> {};

    for (auto val : u) {
      if (edges_.contains(val)) {
        for (auto after : edges_.at(val)) {
          if (seen.contains(after)) {
            return false;
          }
        }
      }

      seen.insert(val);
    }

    return true;
  }

  static long middle(update const& u) { return u.at(u.size() / 2); }

  long part_1() const
  {
    return std::accumulate(
        updates_.begin(), updates_.end(), 0L, [this](auto acc, auto const& u) {
          return acc + (is_sorted(u) ? middle(u) : 0);
        });
  }

  auto compare() const
  {
    return [this](auto x, auto y) {
      return edges_.contains(x) && edges_.at(x).contains(y);
    };
  }

  long part_2() const
  {
    return std::accumulate(
        updates_.begin(), updates_.end(), 0L, [this](auto acc, auto u) {
          if (is_sorted(u)) {
            return acc;
          }

          std::sort(u.begin(), u.end(), compare());
          return acc + middle(u);
        });
  }

private:
  input()
      : edges_ {}
      , updates_ {}
  {
  }

  std::unordered_map<long, std::unordered_set<long>> edges_;
  std::vector<update>                                updates_;
};

int main()
{
  auto in = input::read();
  fmt::print("{}\n", in.part_1());
  fmt::print("{}\n", in.part_2());
}
