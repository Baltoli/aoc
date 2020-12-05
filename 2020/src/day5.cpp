#include <utils/utils.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

constexpr auto pow_2 = std::array {1, 2, 4, 8, 16, 32, 64, 128};

struct seat {
  static constexpr auto row_bits    = 7;
  static constexpr auto column_bits = 3;

  seat(std::string_view spec)
      : row(0)
      , column(0)
  {
    for (auto i = 0; i < row_bits; ++i) {
      auto chr = spec[i];
      row += (chr == 'F' ? 0 : pow_2[row_bits - i - 1]);
    }

    for (auto i = 0; i < column_bits; ++i) {
      auto idx = i + row_bits;
      auto chr = spec[idx];
      column += (chr == 'L' ? 0 : pow_2[column_bits - i - 1]);
    }
  }

  int seat_id() const { return row * 8 + column; }

  bool operator<(seat const& other) const
  {
    return seat_id() < other.seat_id();
  }

  int row;
  int column;
};

int main()
{
  auto seats = std::vector<seat> {};

  utils::for_each_line([&](auto const& line) { seats.emplace_back(line); });

  std::sort(seats.begin(), seats.end());

  std::cout << seats.rbegin()->seat_id() << '\n';

  for (auto i = 0; i < seats.size() - 1; ++i) {
    auto gap = seats[i + 1].seat_id() - seats[i].seat_id();
    if (gap != 1) {
      std::cout << (seats[i].seat_id() + 1) << '\n';
    }
  }
}
