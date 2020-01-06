#include <array>
#include <cassert>
#include <iostream>
#include <string>

bool repeat(std::string const& str)
{
  for (int i = 1; i < str.length(); ++i) {
    if (str[i - 1] == str[i]) {
      return true;
    }
  }
  return false;
}

bool big_group(std::string const& str)
{
  auto runs = std::array<int, 6> {};

  for (int i = 0; i < str.length(); ++i) {
    runs[i] = 1;
    auto c  = str[i];
    for (int j = i + 1; j < str.length() && str[j] == c; ++j) {
      runs[i]++;
    }
  }

  for (int i = 0; i < 6; ++i) {
    if (runs[i] == 2) {
      if (i == 0) {
        return true;
      }
      if (runs[i - 1] != 3) {
        return true;
      }
    }
  }

  return false;
}

bool mono(std::string const& str)
{
  for (int i = 1; i < str.length(); ++i) {
    if (str[i - 1] > str[i]) {
      return false;
    }
  }
  return true;
}

bool valid_password(int num)
{
  auto str = std::to_string(num);
  return big_group(str) && mono(str);
}

void tests()
{
  assert(valid_password(112233));
  assert(!valid_password(123444));
  assert(valid_password(111122));
  assert(!valid_password(111111));
  assert(!valid_password(212345));
  assert(!valid_password(555888));
  assert(valid_password(334789));
}

void solve(std::string const& in)
{
  tests();

  auto low  = std::stoi(in.substr(0, 6));
  auto high = std::stoi(in.substr(7, 6));

  int total = 0;
  for (int i = low; i < high; ++i) {
    total += valid_password(i);
  }
  std::cout << total << '\n';
}

int main() { solve("245318-765747"); }
