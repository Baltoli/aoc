#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>
#include <compare>
#include <limits>
#include <type_traits>
#include <vector>

using namespace ctre::literals;

constexpr auto line_pattern = "(.*) (\\d+)"_ctre;

template <typename Enum, typename Ordering = std::strong_ordering>
Ordering order(Enum a, Enum b)
{
  using underlying = std::underlying_type_t<Enum>;
  return static_cast<underlying>(a) <=> static_cast<underlying>(b);
}

enum class hand_type : uint8_t {
  high_card = 0,
  one_pair,
  two_pair,
  three,
  full_house,
  four,
  five,
};

auto operator<=>(hand_type a, hand_type b) { return order(a, b); }

uint8_t card_rank(char c)
{
  static std::unordered_map<char, uint8_t> ranks = {
      {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3},  {'6', 4},  {'7', 5},  {'8', 6},
      {'9', 7}, {'T', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12},
  };
  return ranks.at(c);
}

uint8_t joker_rank(char c)
{
  static std::unordered_map<char, uint8_t> ranks = {
      {'J', 0}, {'2', 1}, {'3', 2}, {'4', 3},  {'5', 4},  {'6', 5},  {'7', 6},
      {'8', 7}, {'9', 8}, {'T', 9}, {'Q', 10}, {'K', 11}, {'A', 12},
  };
  return ranks.at(c);
}

class hand {
public:
  hand(std::string const& h)
      : joker(false)
      , data_(h)
  {
  }

  bool joker;

  hand_type type() const
  {
    auto os = occurrences();

    auto count = [&](auto n) {
      if (os.contains(n)) {
        return os.at(n);
      }

      return 0L;
    };

    if (count(5) == 1) {
      return hand_type::five;
    } else if (count(4) == 1) {
      return hand_type::four;
    } else if (count(3) == 1 && count(2) == 1) {
      return hand_type::full_house;
    } else if (count(3) == 1) {
      return hand_type::three;
    } else if (count(2) == 2) {
      return hand_type::two_pair;
    } else if (count(2) == 1) {
      return hand_type::one_pair;
    } else if (count(1) == 5) {
      return hand_type::high_card;
    }

    assert(false && "Invalid card occurrences");
  }

  hand_type joker_type() const
  {
    auto cs = counts();

    auto best_count = std::numeric_limits<long>::min();
    auto best_card  = '\0';
    for (auto [card, count] : cs) {
      if (count > best_count && card != 'J') {
        best_count = count;
        best_card  = card;
      }
    }

    auto mod = *this;
    for (auto& c : mod.data_) {
      if (c == 'J') {
        c = best_card;
      }
    }

    return mod.type();
  }

  std::strong_ordering operator<=>(hand const& other)
  {
    auto ty   = joker ? joker_type() : type();
    auto o_ty = joker ? other.joker_type() : other.type();

    if (ty == o_ty) {
      assert(data_.size() == other.data_.size());

      for (auto i = 0u; i < data_.size(); ++i) {
        if (data_[i] != other.data_[i]) {
          if (joker) {
            return joker_rank(data_[i]) <=> joker_rank(other.data_[i]);
          } else {
            return card_rank(data_[i]) <=> card_rank(other.data_[i]);
          }
        }
      }

      return std::strong_ordering::equal;
    } else {
      return ty <=> o_ty;
    }
  }

private:
  std::string data_;

  std::unordered_map<char, long> counts() const
  {
    auto ret = std::unordered_map<char, long> {};
    for (auto c : data_) {
      ret.try_emplace(c, 0);
      ret[c]++;
    }
    return ret;
  }

  std::unordered_map<long, long> occurrences() const
  {
    auto cs = counts();

    auto ret = std::unordered_map<long, long> {};
    for (auto [card, count] : cs) {
      ret.try_emplace(count, 0);
      ret[count]++;
    }

    return ret;
  }
};

void tests()
{
  assert(hand("AAAAA").type() == hand_type::five);
  assert(hand("AA8AA").type() == hand_type::four);
  assert(hand("23332").type() == hand_type::full_house);
  assert(hand("TTT98").type() == hand_type::three);
  assert(hand("23432").type() == hand_type::two_pair);
  assert(hand("A23A4").type() == hand_type::one_pair);
  assert(hand("23456").type() == hand_type::high_card);

  assert(hand("AAAAA") > hand("23456"));
  assert(hand("24442") > hand("23432"));

  assert(hand("33332") > hand("2AAAA"));
  assert(hand("77888") > hand("77788"));
}

std::vector<std::pair<hand, long>> get_input()
{
  return utils::map_lines([](auto const& line) {
    auto [_, h, bid] = line_pattern.match(line);
    return std::pair {hand(h.str()), utils::stol(bid.str())};
  });
}

long part_1(std::vector<std::pair<hand, long>> in)
{
  std::sort(
      in.begin(), in.end(), [](auto& a, auto& b) { return a.first < b.first; });

  auto score = 0L;
  for (auto i = 0u; i < in.size(); ++i) {
    score += in[i].second * (i + 1);
  }

  return score;
}

long part_2(std::vector<std::pair<hand, long>> in)
{
  for (auto& [h, _] : in) {
    h.joker = true;
  }

  return part_1(in);
}

int main()
{
  tests();

  auto in = get_input();
  fmt::print("{}\n", part_1(in));
  fmt::print("{}\n", part_2(in));
}
