#include <utils/utils.h>

#include <iostream>
#include <list>
#include <sstream>
#include <vector>

template <typename It>
void dump(std::list<long> const& xs, It c)
{
  for (auto i = xs.begin(); i != xs.end(); ++i) {
    if (i == c) {
      std::cout << '(' << *i << ") ";
    } else {
      std::cout << *i << ' ';
    }
  }
  std::cout << '\n';
}

std::string format(std::list<long> xs)
{
  auto ss    = std::stringstream {};
  auto start = std::find(xs.begin(), xs.end(), 1);
  std::rotate(xs.begin(), start, xs.end());
  xs.erase(xs.begin());
  for (auto i : xs) {
    ss << i;
  }
  return ss.str();
}

long part_2(std::list<long> const& xs)
{
  auto one = std::find(xs.begin(), xs.end(), 1);
  return *std::next(one) * *std::next(one, 2);
}

std::list<long> get_input()
{
  auto in = std::string {};
  std::getline(std::cin, in);

  auto ret = std::list<long> {};
  for (auto c : in) {
    ret.push_back(c - '0');
  }
  return ret;
}

std::list<long> extend(std::list<long> xs, long target)
{
  auto max  = *std::max_element(xs.begin(), xs.end()) + 1;
  auto init = xs.size();
  for (auto i = 0; i < target - init; ++i, ++max) {
    xs.push_back(max);
  }
  return xs;
}

void step(std::list<long>& xs, long n)
{
  auto ptr = xs.begin();
  auto max = *std::max_element(xs.begin(), xs.end());
  auto min = *std::min_element(xs.begin(), xs.end());

  for (auto i = 0; i < n; ++i) {
    auto current = *ptr;
    auto dest    = *ptr - 1;

    bool any = false;
    for (auto it = ptr; it != std::next(ptr, 4); ++it) {
      if (it == xs.end()) {
        any = true;
        break;
      }
    }

    if (any) {
      std::rotate(xs.begin(), ptr, xs.end());
      ptr = xs.begin();
    }

    auto cut_begin = std::next(ptr);
    auto cut_end   = std::next(ptr, 4);

    while (std::find(cut_begin, cut_end, dest) != cut_end || dest < min) {
      dest -= 1;
      if (dest < min) {
        dest = max;
      }
    }

    auto dest_it = std::next(std::find(xs.begin(), xs.end(), dest));

    for (auto cut_it = cut_begin; cut_it != cut_end; ++cut_it) {
      auto v = *cut_it;
      xs.insert(dest_it, v);
    }

    for (auto cut_it = cut_begin; cut_it != cut_end; ++cut_it) {
      xs.erase(cut_it);
    }

    if (std::next(ptr) == xs.end()) {
      ptr = xs.begin();
    } else {
      ++ptr;
    }
  }
}

int main()
{
  auto in   = get_input();
  auto in_2 = extend(in, 1'000'000);

  step(in, 100);
  std::cout << format(in) << '\n';

  step(in_2, 10'000'000);
  std::cout << part_2(in_2) << '\n';
}
