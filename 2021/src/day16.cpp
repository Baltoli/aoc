#include <utils/utils.h>

#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <string_view>
#include <unordered_map>

uint64_t char_val(char c)
{
  static std::unordered_map<char, uint64_t> table = {
      {'0', 0b0000}, {'1', 0b0001}, {'2', 0b0010}, {'3', 0b0011},
      {'4', 0b0100}, {'5', 0b0101}, {'6', 0b0110}, {'7', 0b0111},
      {'8', 0b1000}, {'9', 0b1001}, {'A', 0b1010}, {'B', 0b1011},
      {'C', 0b1100}, {'D', 0b1101}, {'E', 0b1110}, {'F', 0b1111},
  };

  return table[c];
}

uint64_t extra_bits(int len)
{
  static std::unordered_map<char, uint64_t> table
      = {{0, 0}, {1, 3}, {2, 2}, {3, 1}, {4, 0}};
  return table[len % 4];
}

uint64_t get_bit_range(std::string_view data, int start, int len)
{
  assert(start >= 0 && len <= 64);
  uint64_t ret = 0;

  auto start_char = start / 4;
  auto end_char   = (start + len) / 4;

  for (auto c = start_char; c <= end_char; ++c) {
    ret = ret << 4;
    ret = ret | char_val(data[c]);
  }

  auto bit_len = (end_char + 1) * 4;
  auto right   = bit_len - (start + len);

  ret = ret >> right;
  ret = ret & ((1 << len) - 1);

  return ret;
}

std::pair<uint64_t, bool> get_literal_part(uint64_t bits)
{
  return {bits & 0b1111, bits & 0b10000};
}

struct packet {
  uint64_t version;
  uint64_t type_id;

  uint64_t            value;
  std::vector<packet> children;

  uint64_t validate() const
  {
    return version
           + std::accumulate(
               children.begin(), children.end(), 0,
               [](auto acc, auto const& v) { return acc + v.validate(); });
  }

  uint64_t eval() const
  {
    switch (type_id) {
    case 0:
      return std::accumulate(
          children.begin(), children.end(), 0UL,
          [](auto acc, auto const& p) { return acc + p.eval(); });
    case 1:
      return std::accumulate(
          children.begin(), children.end(), 1UL,
          [](auto acc, auto const& p) { return acc * p.eval(); });
    case 2:
      return std::accumulate(
          children.begin(), children.end(),
          std::numeric_limits<uint64_t>::max(), [](auto acc, auto const& p) {
            auto next = p.eval();
            return next < acc ? next : acc;
          });
    case 3:
      return std::accumulate(
          children.begin(), children.end(),
          std::numeric_limits<uint64_t>::min(), [](auto acc, auto const& p) {
            auto next = p.eval();
            return next > acc ? next : acc;
          });
    case 4: return value;
    case 5: return children[0].eval() > children[1].eval() ? 1 : 0;
    case 6: return children[0].eval() < children[1].eval() ? 1 : 0;
    case 7: return children[0].eval() == children[1].eval() ? 1 : 0;
    default: assert(false && "Bad type ID when eval");
    }
  }
};

std::pair<packet, uint64_t> parse(std::string_view in, uint64_t start = 0)
{
  auto version = get_bit_range(in, start, 3);
  auto type_id = get_bit_range(in, start + 3, 3);

  auto ret = packet {version, type_id, 0, {}};
  auto len = 6;

  if (type_id == 4) {
    auto cursor = start + 6;
    auto val    = uint64_t {0};

    bool next;
    do {
      auto p = get_literal_part(get_bit_range(in, cursor, 5));
      next   = p.second;

      val = val << 4;
      val = val | p.first;

      len += 5;
      cursor += 5;
    } while (next);

    ret.value = val;
  } else {
    auto cursor    = start + 6;
    auto length_id = get_bit_range(in, cursor, 1);
    cursor += 1;
    len += 1;

    if (length_id == 0) {
      auto total_bits = get_bit_range(in, cursor, 15);
      cursor += 15;
      len += 15;

      auto bits_read = 0;
      while (bits_read < total_bits) {
        auto [sub_p, sub_len] = parse(in, cursor);

        len += sub_len;
        cursor += sub_len;
        bits_read += sub_len;

        ret.children.push_back(sub_p);
      }
    } else if (length_id == 1) {
      auto n_packets = get_bit_range(in, cursor, 11);
      cursor += 11;
      len += 11;

      for (auto i = 0; i < n_packets; ++i) {
        auto [sub_p, sub_len] = parse(in, cursor);

        len += sub_len;
        cursor += sub_len;

        ret.children.push_back(sub_p);
      }
    } else {
      assert(false && "Bad length");
    }
  }

  return {ret, len};
}

void tests()
{
  assert(get_bit_range("D2FE28", 0, 3) == 6);
  assert(get_bit_range("D2FE28", 3, 3) == 4);
  assert(get_bit_range("D2FE28", 6, 5) == 0b10111);
  assert(get_bit_range("D2FE28", 11, 5) == 0b11110);
  assert(get_bit_range("D2FE28", 16, 5) == 0b00101);

  auto [p1, l1] = parse("C200B40A82");
  assert(p1.eval() == 3);

  auto [p2, l2] = parse("04005AC33890");
  assert(p2.eval() == 54);

  auto [p3, l3] = parse("880086C3E88112");
  assert(p3.eval() == 7);

  auto [p4, l4] = parse("CE00C43D881120");
  assert(p4.eval() == 9);

  auto [p5, l5] = parse("D8005AC2A8F0");
  assert(p5.eval() == 1);

  auto [p6, l6] = parse("F600BC2D8F");
  assert(p6.eval() == 0);

  auto [p7, l7] = parse("9C005AC2F8F0");
  assert(p7.eval() == 0);

  auto [p8, l8] = parse("9C0141080250320F1802104A08");
  assert(p8.eval() == 1);
}

int main()
{
  tests();

  auto in       = utils::get_lines()[0];
  auto [p, len] = parse(in);

  std::cout << p.validate() << '\n';
  std::cout << p.eval() << '\n';
}
