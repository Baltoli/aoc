#include <utils/ctre.h>
#include <utils/utils.h>

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <numeric>
#include <set>
#include <unordered_map>

using namespace ctre::literals;

constexpr auto line_pattern
    = "(mask = (?<mask>[10X]{36}))|(mem\\[(?<addr>\\d+)\\] = (?<val>\\d+))"_ctre;

constexpr ctll::fixed_string mask_ = "mask";
constexpr ctll::fixed_string addr_ = "addr";
constexpr ctll::fixed_string val_  = "val";

constexpr auto mem_bits = 36;

struct masks {
  std::bitset<mem_bits>      zeroes;
  std::bitset<mem_bits>      ones;
  std::array<char, mem_bits> addr;

  long apply(long val) { return (val | ones.to_ulong()) & zeroes.to_ulong(); }

  std::vector<std::bitset<mem_bits>> apply_addr(long new_addr)
  {
    auto new_bs      = std::bitset<mem_bits>(new_addr);
    auto masked_addr = addr;
    for (auto i = 0; i < new_bs.size(); ++i) {
      if (addr[i] == '0') {
        masked_addr[i] = new_bs[i] ? '1' : '0';
      }
    }

    auto res = std::vector<std::bitset<mem_bits>> {{}};

    for (auto i = 0; i < masked_addr.size(); ++i) {
      auto shift = masked_addr.size() - i - 1;
      auto c     = masked_addr[shift];

      auto news = std::vector<std::bitset<mem_bits>> {};
      for (auto& r : res) {
        if (c == '1' || c == '0') {
          r.set(shift, c == '1');
        } else if (c == 'X') {
          auto r2 = r;
          r2.set(shift, false);
          news.push_back(r2);
          r.set(shift, true);
        }
      }
      for (auto n : news) {
        res.push_back(n);
      }
    }

    return res;
  }
};

masks parse_masks(std::string_view sv)
{
  auto zeroes = std::bitset<mem_bits> {};
  auto ones   = std::bitset<mem_bits> {};
  auto addr   = std::array<char, mem_bits> {};

  for (auto i = 0; i < sv.size() && i < mem_bits; ++i) {
    auto c      = sv[i];
    auto shift  = sv.size() - i - 1;
    addr[shift] = c;

    switch (c) {
    case '0': {
      zeroes.set(shift, false);
      ones.set(shift, false);
      break;
    }
    case '1': {
      zeroes.set(shift, true);
      ones.set(shift, true);
      break;
    }
    case 'X': {
      zeroes.set(shift, true);
      ones.set(shift, false);
      break;
    }
    default: __builtin_unreachable();
    }
  }

  return {zeroes, ones, addr};
}

long part_1(std::vector<std::string> const& lines)
{
  auto current_masks = masks {};
  auto state         = std::unordered_map<long, long> {};

  for (auto const& line : lines) {
    auto m = line_pattern.match(line);

    if (auto mask = m.template get<mask_>()) {
      current_masks = parse_masks(mask);
    } else if (auto addr_s = m.template get<addr_>()) {
      auto addr   = std::stol(addr_s.str());
      auto val    = std::stol(m.template get<val_>().str());
      auto masked = current_masks.apply(val);

      state[addr] = masked;
    } else {
      assert(false);
    }
  }

  auto ret
      = std::accumulate(state.begin(), state.end(), 0L, [](long acc, auto p) {
          return acc + p.second;
        });

  return ret;
}

long part_2(std::vector<std::string> const& lines)
{
  auto current_masks = masks {};
  auto state         = std::unordered_map<long, long> {};

  for (auto const& line : lines) {
    auto m = line_pattern.match(line);

    if (auto mask = m.template get<mask_>()) {
      current_masks = parse_masks(mask);
    } else if (auto addr_s = m.template get<addr_>()) {
      auto addr = std::stol(addr_s.str());
      auto val  = std::stol(m.template get<val_>().str());

      auto addr_masks = current_masks.apply_addr(addr);
      for (auto am : addr_masks) {
        state[am.to_ulong()] = val;
      }
    } else {
      assert(false);
    }
  }

  auto ret
      = std::accumulate(state.begin(), state.end(), 0L, [](long acc, auto p) {
          return acc + p.second;
        });

  return ret;
}

int main()
{
  auto in = utils::get_lines();
  std::cout << part_1(in) << '\n';
  std::cout << part_2(in) << '\n';
}
