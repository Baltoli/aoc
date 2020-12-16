#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <vector>

using namespace ctre::literals;

struct range {
  range() = default;

  range(std::string_view sv)
  {
    constexpr auto pattern = "(\\d+)-(\\d+)"_ctre;

    auto m = pattern.match(sv);
    low    = utils::svtoi(m.template get<1>());
    high   = utils::svtoi(m.template get<2>());
  }

  bool contains(int i) const { return i >= low && i <= high; }

  int low;
  int high;
};

struct field {
  field(std::string_view line)
  {
    constexpr auto pattern = "(.*): (\\d+-\\d+) or (\\d+-\\d+)"_ctre;

    auto m = pattern.match(line);
    name   = m.template get<1>().str();
    first  = range(m.template get<2>());
    second = range(m.template get<3>());
  }

  bool valid_data(int i) const
  {
    return first.contains(i) || second.contains(i);
  }

  std::string name;
  range       first;
  range       second;
};

struct ticket {
  ticket()
      : values {}
  {
  }

  ticket(std::string const& line)
      : values {}
  {
    for (auto&& i : utils::split(line, ",")) {
      values.push_back(std::stoi(i));
    }
  }

  std::vector<int> values;
};

struct input {
  std::vector<field>  fields;
  ticket              my_ticket;
  std::vector<ticket> others;
};

input get_input()
{
  constexpr auto skip = "(your ticket:)|(nearby tickets:)"_ctre;

  auto fields = std::vector<field> {};
  auto mine   = ticket {};
  auto others = std::vector<ticket> {};

  auto section = 0;
  utils::for_each_line([&](auto const& line) {
    if (skip.match(line)) {
      return;
    }

    if (line.empty()) {
      ++section;
      return;
    }

    switch (section) {
    case 0:
      fields.emplace_back(line);
      break;
    case 1:
      mine = ticket(line);
      break;
    case 2:
      others.emplace_back(line);
      break;
    default:
      assert(false);
    }
  });

  return {fields, mine, others};
}

int part_1(input const& in)
{
  auto sum = 0;

  for (auto const& t : in.others) {
    for (auto val : t.values) {
      auto none = std::none_of(
          in.fields.begin(), in.fields.end(),
          [val](auto const& f) { return f.valid_data(val); });

      if (none) {
        sum += val;
      }
    }
  }

  return sum;
}

int main()
{
  auto in = get_input();
  std::cout << part_1(in) << '\n';
}
