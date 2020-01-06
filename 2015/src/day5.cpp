#include <utils/utils.h>

#include <algorithm>
#include <array>
#include <string>

bool vowels(std::string const& s)
{
  auto count = 0;
  for (auto c : {'a', 'e', 'i', 'o', 'u'}) {
    count += std::count(s.begin(), s.end(), c);
  }
  return count >= 3;
}

bool consecutive(std::string const& s)
{
  for (int i = 0; i < s.size() - 1; ++i) {
    if (s[i] == s[i + 1]) {
      return true;
    }
  }
  return false;
}

bool bad_strings(std::string const& s)
{
  auto bads = std::array {"ab", "cd", "pq", "xy"};
  return std::none_of(bads.begin(), bads.end(), [&](auto bad) {
    return s.find(bad) != std::string::npos;
  });
}

bool nice(std::string const& s)
{
  return vowels(s) && consecutive(s) && bad_strings(s);
}

bool pairs(std::string const& s)
{
  for (int i = 0; i < s.size() - 1; ++i) {
    auto sub = s.substr(i, 2);
    for (int j = 0; j < s.size() - 1; ++j) {
      if (std::abs(i - j) > 1) {
        auto sub2 = s.substr(j, 2);
        if (sub == sub2) {
          return true;
        }
      }
    }
  }
  return false;
}

bool threes(std::string const& s)
{
  for (int i = 0; i < s.size() - 2; ++i) {
    if (s[i] == s[i + 2]) {
      return true;
    }
  }
  return false;
}

bool nice_v2(std::string const& s) { return pairs(s) && threes(s); }

int main()
{
  int nices = 0;
  int v2s   = 0;

  utils::for_each_line([&](auto line) {
    nices += nice(line);
    v2s += nice_v2(line);
  });

  std::cout << nices << '\n';
  std::cout << v2s << '\n';
}
