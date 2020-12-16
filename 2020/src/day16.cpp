#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <unordered_set>
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

  input clean() const
  {
    auto valid_ts = std::vector<ticket> {};
    for (auto const& t : others) {
      auto any_none = false;
      for (auto val : t.values) {
        auto none
            = std::none_of(fields.begin(), fields.end(), [&](auto const& f) {
                return f.valid_data(val);
              });
        if (none) {
          any_none = true;
        }
      }

      if (!any_none) {
        valid_ts.push_back(t);
      }
    }
    return {fields, my_ticket, valid_ts};
  }
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

long part_2(input const& in)
{
  auto cand_sets = std::vector<std::unordered_set<std::string>>(
      in.fields.size(), std::unordered_set<std::string> {});

  for (auto f : in.fields) {
    for (auto& s : cand_sets) {
      s.insert(f.name);
    }
  }

  auto check = [&](auto const& t) {
    for (auto i = 0; i < t.values.size(); ++i) {
      for (auto f : in.fields) {
        if (!f.valid_data(t.values.at(i))) {
          cand_sets.at(i).erase(f.name);
        }
      }
    }
  };

  check(in.my_ticket);
  for (auto const& t : in.others) {
    check(t);
  }

  auto not_done = [&] {
    return std::any_of(cand_sets.begin(), cand_sets.end(), [](auto const& cs) {
      return cs.size() != 1;
    });
  };

  while (not_done()) {
    for (auto i = 0; i < cand_sets.size(); ++i) {
      auto& cs = cand_sets.at(i);
      if (cs.size() == 1) {
        auto unique = *cs.begin();
        for (auto j = 0; j < cand_sets.size(); ++j) {
          if (i != j) {
            cand_sets.at(j).erase(unique);
          }
        }
      }
    }
  }

  long prod = 1;
  for (auto i = 0; i < cand_sets.size(); ++i) {
    if (cand_sets.at(i).begin()->find("departure") == 0) {
      prod *= in.my_ticket.values.at(i);
    }
  }

  return prod;
}

int main()
{
  auto in = get_input();
  std::cout << part_1(in) << '\n';
  std::cout << part_2(in.clean()) << '\n';
}
