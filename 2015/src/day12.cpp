#include <utils/picojson.h>

#include <algorithm>
#include <iostream>
#include <numeric>

bool has_red_value(picojson::object const& obj)
{
  return std::any_of(obj.begin(), obj.end(), [](auto const& p) {
    if (p.second.template is<std::string>()) {
      return p.second.template get<std::string>() == "red";
    } else {
      return false;
    }
  });
}

int sum(picojson::value const& v, bool ignore_red)
{
  if (v.is<double>()) {
    return v.get<double>();
  }

  if (v.is<picojson::array>()) {
    auto arr = v.get<picojson::array>();
    return std::accumulate(
        arr.begin(), arr.end(), 0,
        [&](auto acc, auto const& v) { return acc + sum(v, ignore_red); });
  }

  if (v.is<picojson::object>()) {
    auto obj = v.get<picojson::object>();
    if (ignore_red && has_red_value(obj)) {
      return 0;
    }

    return std::accumulate(
        obj.begin(), obj.end(), 0, [&](auto acc, auto const& v) {
          return acc + sum(v.second, ignore_red);
        });
  }

  return 0;
}

int main()
{
  auto in = std::string {};
  std::getline(std::cin, in);

  auto v   = picojson::value {};
  auto err = picojson::parse(v, in);

  std::cout << sum(v, false) << '\n';
  std::cout << sum(v, true) << '\n';
}
