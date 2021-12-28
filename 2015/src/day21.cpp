#include <utils/utils.h>

#include <fmt/format.h>

#include <array>
#include <limits>
#include <numeric>
#include <string_view>

struct item {
  std::string_view name;
  int              cost;
  int              damage;
  int              armor;
};

constexpr std::array weapons
    = {item {"Dagger", 8, 4, 0}, item {"Shortsword", 10, 5, 0},
       item {"Warhammer", 25, 6, 0}, item {"Longsword", 40, 7, 0},
       item {"Greataxe", 74, 8, 0}};

constexpr std::array armors
    = {item {"None", 0, 0, 0},        item {"Leather", 13, 0, 1},
       item {"Chainmail", 31, 0, 2},  item {"Splintmail", 53, 0, 3},
       item {"Bandedmail", 75, 0, 4}, item {"Platemail", 102, 0, 5}};

constexpr std::array rings
    = {item {"None", 0, 0, 0},        item {"None", 0, 0, 0},
       item {"Damage +1", 25, 1, 0},  item {"Damage +2", 50, 2, 0},
       item {"Damage +3", 100, 3, 0}, item {"Defense +1", 20, 0, 1},
       item {"Defense +2", 40, 0, 2}, item {"Defense +3", 80, 0, 3}};

class entity {
public:
  entity(int h, int d, int a)
      : base_hp_(h)
      , hp_(h)
      , base_damage_(d)
      , base_armor_(a)
      , items_ {}
  {
  }

  static entity load_boss()
  {
    auto values = utils::map_lines([](auto const& line) {
      return utils::stoi(utils::split(line, ": ")[1]);
    });

    return entity(values.at(0), values.at(1), values.at(2));
  }

  void add(item i) { items_.push_back(i); }

  int damage() const
  {
    return base_damage_
           + std::accumulate(
               items_.begin(), items_.end(), 0,
               [](auto acc, auto const& i) { return acc + i.damage; });
  }

  int armor() const
  {
    return base_armor_
           + std::accumulate(
               items_.begin(), items_.end(), 0,
               [](auto acc, auto const& i) { return acc + i.armor; });
  }

  int hp() const { return hp_; }

  void attack(entity& other)
  {
    int real_damage = std::max(1, damage() - other.armor());
    other.hp_ -= real_damage;
  }

private:
  int base_hp_;
  int hp_;
  int base_damage_;
  int base_armor_;

  std::vector<item> items_;
};

bool simulate(entity a, entity b)
{
  while (true) {
    a.attack(b);
    if (b.hp() <= 0) {
      return true;
    }

    b.attack(a);
    if (a.hp() <= 0) {
      return false;
    }
  }
}

int main()
{
  auto boss   = entity::load_boss();
  auto player = entity(100, 0, 0);

  auto best_cost  = std::numeric_limits<int>::max();
  auto worst_cost = std::numeric_limits<int>::min();

  for (auto w : weapons) {
    for (auto a : armors) {
      for (auto i = 0; i < rings.size(); ++i) {
        for (auto j = 0; j < rings.size(); ++j) {
          if (i != j) {
            auto pc = player;

            pc.add(w);
            pc.add(a);
            pc.add(rings[i]);
            pc.add(rings[j]);

            auto cost = w.cost + a.cost + rings[i].cost + rings[j].cost;
            if (simulate(pc, boss)) {
              best_cost = std::min(best_cost, cost);
            } else {
              worst_cost = std::max(worst_cost, cost);
            }
          }
        }
      }
    }
  }

  fmt::print("{}\n", best_cost);
  fmt::print("{}\n", worst_cost);
}

template <>
struct fmt::formatter<item> {
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const item& i, FormatContext& ctx) -> decltype(ctx.out())
  {
    return format_to(
        ctx.out(), "{}: cost={} damage={} armor={}", i.name, i.cost, i.damage,
        i.armor);
  }
};
