#include <utils/utils.h>

#include <fmt/format.h>

#include <optional>
#include <queue>

class entity {
public:
  entity(int h, int d, int a, int m)
      : hp_(h)
      , damage_(d)
      , armor_(a)
      , mana_(m)
  {
  }

  static entity load_boss()
  {
    auto values = utils::map_lines([](auto const& line) {
      return utils::stoi(utils::split(line, ": ")[1]);
    });

    return entity(values.at(0), values.at(1), 0, 0);
  }

  int& shield() { return shield_; }
  int& poison() { return poison_; }
  int& recharge() { return recharge_; }

  int armor() const { return armor_ + (shield_ > 0 ? 7 : 0); }
  int damage() const { return damage_; }
  int total_mana() const { return total_mana_; }

  void apply_effects()
  {
    if (poison_ > 0) {
      hp_ -= 3;
    }

    if (recharge_ > 0) {
      mana_ += 101;
    }

    shield_   = std::max(0, shield_ - 1);
    poison_   = std::max(0, poison_ - 1);
    recharge_ = std::max(0, recharge_ - 1);
  }

  void apply_difficulty()
  {
    if (hard) {
      hp_ -= 1;
    }
  }

  int& hp() { return hp_; }
  int  mana() const { return mana_; }

  void use_mana(int m)
  {
    mana_ -= m;
    total_mana_ += m;
  }

  bool hard = false;

private:
  int hp_;
  int damage_;
  int armor_;
  int mana_;

  int total_mana_ = 0;

  int shield_   = 0;
  int poison_   = 0;
  int recharge_ = 0;
};

struct state {
  entity player;
  entity boss;
  bool   player_turn;
};

int simulate(entity start_player, entity start_boss)
{
  auto best_mana = std::numeric_limits<int>::max();

  auto queue = std::queue<state> {};
  queue.push({start_player, start_boss, true});

  while (!queue.empty()) {
    auto [player, boss, p_turn] = queue.front();
    queue.pop();

    if (p_turn) {
      player.apply_difficulty();
    }

    if (player.hp() <= 0 || player.mana() <= 0) {
      continue;
    }

    player.apply_effects();
    boss.apply_effects();

    if (boss.hp() <= 0) {
      best_mana = std::min(best_mana, player.total_mana());
      continue;
    }

    if (player.total_mana() >= best_mana) {
      continue;
    }

    if (p_turn) {
      if (player.mana() >= 53) {
        // Magic missile
        auto np = player;
        auto nb = boss;

        np.use_mana(53);
        nb.hp() -= 4;

        queue.push({np, nb, false});
      }

      if (player.mana() >= 73) {
        // Drain
        auto np = player;
        auto nb = boss;

        np.use_mana(73);
        nb.hp() -= 2;
        np.hp() += 2;

        queue.push({np, nb, false});
      }

      if (player.mana() >= 113 && player.shield() == 0) {
        // Shield
        auto np = player;

        np.use_mana(113);
        np.shield() = 6;

        queue.push({np, boss, false});
      }

      if (player.mana() >= 173 && boss.poison() == 0) {
        // Poison
        auto np = player;
        auto nb = boss;

        np.use_mana(173);
        nb.poison() = 6;

        queue.push({np, nb, false});
      }

      if (player.mana() >= 229 && player.recharge() == 0) {
        // Recharge
        auto np = player;

        np.use_mana(229);
        np.recharge() = 5;

        queue.push({np, boss, false});
      }
    } else {
      auto np = player;
      np.hp() -= std::max(1, boss.damage() - np.armor());

      queue.push({np, boss, true});
    }
  }

  return best_mana;
}

int main()
{
  auto start_player = entity(50, 0, 0, 500);
  auto start_boss   = entity::load_boss();

  fmt::print("{}\n", simulate(start_player, start_boss));

  start_player.hard = true;
  fmt::print("{}\n", simulate(start_player, start_boss));
}
