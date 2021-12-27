#include <utils/utils.h>

#include <fmt/format.h>

#include <array>
#include <queue>
#include <unordered_map>
#include <unordered_set>

/*

#############
#...........#
###0#2#4#6###
  #1#3#5#7#
  #########

*/

struct state;

namespace std {
template <>
struct hash<state> {
  size_t operator()(state const&) const;
};
} // namespace std

struct state {
  static const std::unordered_map<char, int> costs;
  static const std::unordered_map<char, int> room_max;
  static constexpr std::array<char, 8>       targets
      = {'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D'};
  static constexpr int room_size = 2;

  std::array<char, 11> hallway = {};
  std::array<char, 8>  rooms   = {};
  int                  energy  = 0;

  static state load()
  {
    auto ret   = state();
    auto lines = utils::get_lines();

    auto idx = 1;
    for (auto& c : ret.hallway) {
      c = lines[1][idx++];
    }

    ret.rooms = {lines[2][3], lines[3][3], lines[2][5], lines[3][5],
                 lines[2][7], lines[3][7], lines[2][9], lines[3][9]};
    return ret;
  }

  bool wins() const { return rooms == targets; }

  std::vector<state> next_steps() const
  {
    auto ret = std::vector<state> {};

    if (wins()) {
      return ret;
    }

    for (auto r = 0; r < rooms.size(); ++r) {
      auto tgt        = targets.at(r);
      auto all_sorted = true;
      for (auto rb = r; rb <= room_max.at(tgt); ++rb) {
        all_sorted = all_sorted && rooms[rb] == tgt;
      }

      if (rooms[r] != '.' && !all_sorted) {
        for (auto h = 0; h < hallway.size(); ++h) {
          auto reachable
              = hallway[h] == '.' && reachable_from(room_hall_entry(r), h)
                && (h != 2 && h != 4 && h != 6 && h != 8) && room_exit_clear(r);

          if (reachable) {
            auto next = *this;
            next.energy += room_hall_distance(r, h) * costs.at(rooms[r]);
            std::swap(next.rooms[r], next.hallway[h]);
            ret.push_back(next);
          }
        }
      }
    }

    for (auto h = 0; h < hallway.size(); ++h) {
      if (hallway[h] != '.') {
        auto pod   = hallway[h];
        auto r_max = room_max.at(pod);

        if (rooms[r_max] == '.' && reachable_from(h, room_hall_entry(r_max))
            && room_exit_clear(r_max)) {
          auto next = *this;
          next.energy += room_hall_distance(r_max, h) * costs.at(pod);
          std::swap(next.rooms[r_max], next.hallway[h]);
          ret.push_back(next);
        } else if (
            rooms[r_max - 1] == '.'
            && reachable_from(h, room_hall_entry(r_max - 1))
            && room_exit_clear(r_max - 1) && rooms[r_max] == pod) {
          auto next = *this;
          next.energy += room_hall_distance(r_max - 1, h) * costs.at(pod);
          std::swap(next.rooms[r_max - 1], next.hallway[h]);
          ret.push_back(next);
        }
      }
    }

    return ret;
  }

  int minimum_cost() const
  {
    auto min_cost = std::numeric_limits<int>::max();
    auto visited  = std::unordered_set<state> {};

    auto q_cmp
        = [](auto const& a, auto const& b) { return a.energy > b.energy; };

    auto queue
        = std::priority_queue<state, std::vector<state>, decltype(q_cmp)>(
            q_cmp);

    queue.push(*this);

    while (!queue.empty()) {
      auto current = queue.top();
      queue.pop();

      if (visited.contains(current) || current.energy > min_cost) {
        continue;
      }

      if (current.wins()) {
        min_cost = std::min(min_cost, current.energy);
      } else {
        for (auto next : current.next_steps()) {
          queue.push(next);
        }
      }

      visited.insert(current);
    }

    return min_cost;
  }

  bool room_exit_clear(int room) const
  {
    auto base = (room / 2) * 2;
    for (auto r = base; r < room; ++r) {
      if (rooms[r] != '.') {
        return false;
      }
    }
    return true;
  }

  bool reachable_from(int start, int end) const
  {
    for (auto i = std::min(start, end); i <= std::max(start, end); ++i) {
      if (i != start && hallway[i] != '.') {
        return false;
      }
    }

    return true;
  }

  int room_hall_entry(int room) const { return ((room / 2) + 1) * 2; }

  int room_hall_distance(int room, int hall) const
  {
    int exit_cost = (room % 2) + 1;
    return exit_cost + std::abs(room_hall_entry(room) - hall);
  }

  bool operator==(state const& other) const
  {
    return std::tie(energy, rooms, hallway)
           == std::tie(other.energy, other.rooms, other.hallway);
  }
};

int main()
{
  auto s = state::load();
  fmt::print("{}\n", s.minimum_cost());
}

template <>
struct fmt::formatter<state> {
  constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const state& s, FormatContext& ctx) -> decltype(ctx.out())
  {
    auto str = R"(#############
#{}#
###{}#{}#{}#{}###
  #{}#{}#{}#{}#
  #########)";

    return format_to(
        ctx.out(), str, fmt::join(s.hallway, ""), s.rooms[0], s.rooms[2],
        s.rooms[4], s.rooms[6], s.rooms[1], s.rooms[3], s.rooms[5], s.rooms[7]);
  }
};

size_t std::hash<state>::operator()(state const& s) const
{
  auto seed = size_t {0};

  for (auto r : s.rooms) {
    utils::hash_combine(seed, r);
  }
  for (auto h : s.hallway) {
    utils::hash_combine(seed, h);
  }

  return seed;
}

const std::unordered_map<char, int> state::costs
    = {{'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};

const std::unordered_map<char, int> state::room_max
    = {{'A', 1}, {'B', 3}, {'C', 5}, {'D', 7}};
