#include <intcode.h>

#include <cassert>
#include <iostream>
#include <string>

class beam {
public:
  beam(std::string const&);

  bool test(int x, int y) const;
  bool test_square(int x, int y, int size) const;

  int x_extent(int y) const;

private:
  ic::computer computer_;
};

beam::beam(std::string const& prog)
    : computer_(prog)
{
}

bool beam::test(int x, int y) const
{
  auto new_c = computer_;
  new_c.input(x);
  new_c.input(y);
  return new_c.run();
}

bool beam::test_square(int x, int y, int size) const
{
  return test(x + size - 1, y) && test(x, y + size - 1);
}

int beam::x_extent(int y) const
{
  assert(y > 7 && "Don't use at start");

  int x = 0;

  while (!test(x, y)) {
    ++x;
  }

  while (test(x, y)) {
    ++x;
  }

  return x - 1;
}

void part_1(beam& b)
{
  auto total = 0;
  for (int x = 0; x < 50; ++x) {
    for (int y = 0; y < 50; ++y) {
      total += b.test(x, y);
    }
  }
  std::cout << total << '\n';
}

void part_2(beam& b)
{
  constexpr auto size = 100;

  auto y = 10;
  auto x = b.x_extent(y);

  while (true) {
    auto top_left_x = x - (size - 1);

    if (b.test_square(top_left_x, y, size)) {
      std::cout << ((top_left_x * 10'000) + y) << '\n';
      return;
    } else {
      ++y;
      while (b.test(x, y)) {
        ++x;
      }
      --x;
    }
  }
}

int main()
{
  std::string line;
  std::getline(std::cin, line);
  auto b = beam(line);

  part_1(b);
  part_2(b);
}
