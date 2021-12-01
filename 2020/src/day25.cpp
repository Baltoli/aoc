#include <utils/utils.h>

#include <cassert>
#include <iostream>
#include <vector>

constexpr long subject = 7;
constexpr long mod     = 20201227;

struct input {
  long card;
  long door;
};

input get_input()
{
  auto in = std::string {};

  std::getline(std::cin, in);
  auto c = std::stol(in);

  std::getline(std::cin, in);
  auto d = std::stol(in);

  return input {c, d};
}

long transform(long sub, long loop)
{
  auto ret = 1;

  for (auto i = 0; i < loop; ++i) {
    ret = (sub * ret) % mod;
  }

  return ret;
}

int main()
{
  auto in = get_input();
  /* auto in = input {5764801, 17807724}; */

  auto card_loop = 0;
  auto door_loop = 0;
  auto loop      = 1;

  auto cache = std::vector<long>(mod, 0);
  cache[0]   = 1;

  while (card_loop == 0 || door_loop == 0) {
    cache[loop] = (cache[loop - 1] * subject) % mod;

    if (cache[loop] == in.card) {
      card_loop = loop;
    }

    if (cache[loop] == in.door) {
      door_loop = loop;
    }

    ++loop;
  }

  auto k1 = transform(in.card, door_loop);
  auto k2 = transform(in.door, card_loop);

  assert(k1 == k2);

  std::cout << k1 << '\n';
}
