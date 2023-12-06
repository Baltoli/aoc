#include <utils/ctre.h>
#include <utils/utils.h>

#include <fmt/format.h>

#include <cassert>

constexpr long distance(long held, long duration)
{
  if (held <= 0 || held >= duration) {
    return 0;
  }

  auto travel_time = duration - held;
  return travel_time * held;
}

long lower_bound(long record, long duration)
{
  for (auto i = 0L; i < duration; ++i) {
    if (distance(i, duration) > record) {
      return i;
    }
  }

  return duration + 1;
}

long upper_bound(long record, long duration)
{
  for (auto i = duration; i >= 0; --i) {
    if (distance(i, duration) > record) {
      return i;
    }
  }

  return duration + 1;
}

long margin(long record, long duration)
{
  auto upper = upper_bound(record, duration);
  auto lower = lower_bound(record, duration);

  assert(upper >= lower);

  return upper - lower + 1;
}

void tests()
{
  static_assert(distance(0, 7) == 0);
  static_assert(distance(1, 7) == 6);
  static_assert(distance(2, 7) == 10);
  static_assert(distance(3, 7) == 12);
  static_assert(distance(4, 7) == 12);
  static_assert(distance(5, 7) == 10);
  static_assert(distance(6, 7) == 6);
  static_assert(distance(7, 7) == 0);

  assert(lower_bound(9, 7) == 2);
  assert(lower_bound(40, 15) == 4);
  assert(lower_bound(200, 30) == 11);

  assert(upper_bound(9, 7) == 5);
  assert(upper_bound(40, 15) == 11);
  assert(upper_bound(200, 30) == 19);

  assert(margin(9, 7) == 4);
  assert(margin(40, 15) == 8);
  assert(margin(200, 30) == 9);
}

long part_1()
{
  return margin(249, 41) * margin(1362, 77) * margin(1127, 70)
         * margin(1011, 96);
}

long part_2() { return margin(249136211271011, 41777096); }

int main()
{
  tests();
  fmt::print("{}\n", part_1());
  fmt::print("{}\n", part_2());
}
