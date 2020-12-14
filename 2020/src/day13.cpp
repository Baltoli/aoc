#include <utils/utils.h>

#include <algorithm>
#include <iostream>
#include <string>

long read_time()
{
  auto in = std::string {};
  std::getline(std::cin, in);
  return std::stoi(in);
}

std::vector<long> read_ids()
{
  auto in = std::string {};
  std::getline(std::cin, in);
  auto list = utils::split(in, ",");

  auto ids = std::vector<long> {};
  std::for_each(list.begin(), list.end(), [&ids](auto const& entry) {
    if (entry == "x") {
      ids.push_back(0);
    } else {
      ids.push_back(std::stol(entry));
    }
  });

  return ids;
}

long waiting_time(long now, long id)
{
  auto next = ((now / id) + 1) * id;
  return next - now;
}

long part_1(long t, std::vector<long> const& ids)
{
  auto min_wait = std::numeric_limits<long>::max();
  auto min_id   = 0;
  for (auto i : ids) {
    if (i != 0) {
      auto w = waiting_time(t, i);
      if (w < min_wait) {
        min_wait = w;
        min_id   = i;
      }
    }
  }
  return min_wait * min_id;
}

std::pair<long, long> egcd(long a, long b)
{
  auto old_r = a;
  auto r     = b;

  auto old_s = 1L;
  auto s     = 0L;

  auto old_t = 0L;
  auto t     = 1L;

  while (r != 0) {
    auto quot          = old_r / r;
    std::tie(old_r, r) = std::make_tuple(r, old_r - quot * r);
    std::tie(old_s, s) = std::make_tuple(s, old_s - quot * s);
    std::tie(old_t, t) = std::make_tuple(t, old_t - quot * t);
  }

  return {old_s, old_t};
}

long part_2(std::vector<long> const& ids)
{
  auto mods = std::vector<long> {};
  auto as   = std::vector<long> {};
  for (auto i = 0; i < ids.size(); ++i) {
    if (ids[i] != 0) {
      mods.push_back(ids[i]);

      auto rem = ids[i] - i;
      while (rem < 0) {
        rem += ids[i];
      }
      as.push_back(rem);
    }
  }

  auto Ns = std::vector<long> {};
  auto Ms = std::vector<long> {};
  for (auto i = 0; i < mods.size(); ++i) {
    auto prod = 1L;
    for (auto j = 0; j < mods.size(); ++j) {
      if (i != j) {
        prod *= mods[j];
      }
    }
    Ns.push_back(prod);

    auto [M, m] = egcd(Ns[i], mods[i]);
    Ms.push_back(M);
  }

  auto sum  = 0L;
  auto prod = 1L;
  for (auto i = 0; i < mods.size(); ++i) {
    sum += as[i] * Ns[i] * Ms[i];
    prod *= mods[i];
  }

  auto scale = sum / prod;
  return sum - ((scale - (sum < 0 ? 1 : 0)) * prod);
}

int main()
{
  auto t   = read_time();
  auto ids = read_ids();

  std::cout << part_1(t, ids) << '\n';
  std::cout << part_2(ids) << '\n';
}
