#include <utils/ctre.h>
#include <utils/utils.h>

#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace ctre::literals;

struct result {
  bool             success;
  std::string_view rest;
};

struct node {
  virtual ~node()                              = default;
  virtual result match(std::string_view) const = 0;
};

using regex = std::shared_ptr<node>;

struct character final : public node {
  character(char ch)
      : c(ch)
  {
  }

  result match(std::string_view sv) const override
  {
    if (sv.length() > 0 && sv[0] == c) {
      return {true, sv.substr(1)};
    } else {
      return {false, sv};
    }
  }

  char c;
};

struct seq final : public node {
  seq(std::vector<regex> rs)
      : parts(rs)
  {
  }

  result match(std::string_view sv) const override
  {
    auto ret = result {true, sv};
    for (auto re : parts) {
      ret = re->match(ret.rest);
      if (!ret.success) {
        return {false, sv};
      }
    }
    return ret;
  }

  std::vector<regex> parts;
};

struct choice final : public node {
  choice(regex l, regex r)
      : left(l)
      , right(r)
  {
  }

  result match(std::string_view sv) const override
  {
    auto [l_suc, l_res] = left->match(sv);
    if (l_suc) {
      return {l_suc, l_res};
    } else {
      return right->match(sv);
    }
  }

  regex left;
  regex right;
};

struct rule_set {
  void add(std::string_view s)
  {
    constexpr auto rule_pattern = "(\\d+): (.*)"_ctre;

    auto m    = rule_pattern.match(s);
    auto id   = utils::svtoi(m.template get<1>());
    auto rest = std::string(m.template get<2>());

    sources[id] = rest;
  }

  regex new_root()
  {
    constexpr auto reps    = 13;
    constexpr auto max_len = 100;

    regex ret = nullptr;

    for (auto n_8 = 1; n_8 < reps; ++n_8) {
      for (auto n_11 = 1; n_11 < reps; ++n_11) {
        if (n_8 * 8 + n_11 * 16 < max_len) {
          auto parts = std::vector<regex> {};

          for (auto i = 0; i < n_8 + n_11; ++i) {
            parts.push_back(get(42));
          }

          for (auto i = 0; i < n_11; ++i) {
            parts.push_back(get(31));
          }

          if (!ret) {
            ret = std::make_shared<seq>(parts);
          } else {
            ret = std::make_shared<choice>(ret, std::make_shared<seq>(parts));
          }
        }
      }
    }

    return ret;
  }

  regex const& get(int id)
  {
    if (compiled.find(id) == compiled.end()) {
      compiled[id] = compile(id);
    }

    return compiled.at(id);
  }

  regex parse_sequence(std::string const& src)
  {
    auto lit_pattern = "\"([a-z])\""_ctre;
    auto ref_pattern = "(\\d+)"_ctre;

    auto components = utils::split(src, " ");
    auto seq_v      = std::vector<regex> {};

    for (auto const& c : components) {
      if (auto lm = lit_pattern.match(c)) {
        auto c = std::make_shared<character>(lm.template get<1>().str()[0]);
        seq_v.push_back(c);
      } else if (auto rm = ref_pattern.match(c)) {
        auto id = utils::svtoi(rm.template get<1>());
        seq_v.push_back(get(id));
      } else {
        assert(false && "Bad seq");
      }
    }

    return std::make_shared<seq>(seq_v);
  }

  regex compile(int id)
  {
    constexpr auto choice_pattern = "(.*) \\| (.*)"_ctre;

    auto const& src = sources.at(id);
    if (auto cm = choice_pattern.match(src)) {
      auto lhs = parse_sequence(std::string(cm.template get<1>()));
      auto rhs = parse_sequence(std::string(cm.template get<2>()));
      return std::make_shared<choice>(lhs, rhs);
    } else {
      return parse_sequence(src);
    }
  }

  regex const& root() { return get(0); }

  std::unordered_map<int, std::string> sources  = {};
  std::unordered_map<int, regex>       compiled = {};
};

int main()
{
  auto sum   = 0;
  auto sum_2 = 0;
  auto rules = rule_set {};

  auto state = 0;
  utils::for_each_line([&](auto const& line) {
    if (line.empty()) {
      ++state;
      return;
    }

    if (state == 0) {
      rules.add(line);
    } else {
      auto const& root = rules.root();
      auto [suc, rest] = root->match(line);
      if (suc && rest.empty()) {
        ++sum;
      }

      auto const& root_2 = rules.new_root();
      auto [suc2, rest2] = root_2->match(line);
      if (suc2 && rest2.empty()) {
        ++sum_2;
      }
    }
  });

  std::cout << sum << '\n';
  std::cout << sum_2 << '\n';
}
