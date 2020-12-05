#include <iostream>
#include <string>

std::pair<char, bool> wrap(char c)
{
  return {(((c - 97) + 1) % 26) + 97, c == 'z'};
}

void increment(std::string& pass)
{
  bool carry = true;
  for (int i = pass.size() - 1; i >= 0 && carry; --i) {
    auto [chr, next_carry] = wrap(pass[i]);
    pass[i]                = chr;
    carry                  = next_carry;
  }
}

bool has_run(std::string_view pass)
{
  for (auto i = 0; i < pass.size() - 2; ++i) {
    auto g1 = pass[i + 1] - pass[i];
    auto g2 = pass[i + 2] - pass[i + 1];
    if (g1 == 1 && g2 == 1) {
      return true;
    }
  }
  return false;
}

bool bad_letters(std::string_view pass)
{
  for (auto c : pass) {
    if (c == 'i' || c == 'o' || c == 'l') {
      return true;
    }
  }
  return false;
}

bool pairs(std::string_view pass)
{
  auto n = 0;

  for (auto i = 0; i < pass.size() - 1; ++i) {
    auto c1 = pass[i];
    auto c2 = pass[i + 1];

    if (c1 == c2) {
      n++;
      i++;
    }
  }

  return n >= 2;
}

bool valid(std::string_view pass)
{
  return has_run(pass) && !bad_letters(pass) && pairs(pass);
}

std::string next(std::string pass)
{
  while (!valid(pass)) {
    increment(pass);
  }
  return pass;
}

int main()
{
  auto pass = std::string {};
  std::getline(std::cin, pass);

  pass = next(pass);
  std::cout << pass << '\n';

  increment(pass);
  std::cout << next(pass) << '\n';
}
