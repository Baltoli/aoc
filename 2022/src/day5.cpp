#include <utils/ctre.h>
#include <utils/utils.h>

#include <iostream>
#include <sstream>
#include <vector>

struct move {
  int amount;
  int start;
  int end;

  static move parse(std::string_view line)
  {
    auto m = ctre::match<R"(move (\d+) from (\d+) to (\d+))">(line);
    return {
        utils::svtoi(m.get<1>()), utils::svtoi(m.get<2>()) - 1,
        utils::svtoi(m.get<3>()) - 1};
  }
};

class state {
public:
  static std::pair<state, state> load();

  void apply(move const& m, bool part_2)
  {
    auto& from_s = stacks_.at(m.start);
    auto& to_s   = stacks_.at(m.end);

    if (!part_2) {
      std::reverse(from_s.end() - m.amount, from_s.end());
    }
    std::copy(from_s.end() - m.amount, from_s.end(), std::back_inserter(to_s));
    from_s.erase(from_s.end() - m.amount, from_s.end());
  }

  std::string message() const
  {
    auto ss = std::stringstream {};
    for (auto const& st : stacks_) {
      ss << st.back();
    }
    return ss.str();
  }

private:
  state(int n)
      : stacks_(n, std::vector<char> {})
  {
  }

  std::vector<std::vector<char>> stacks_ = {};
};

std::pair<state, state> state::load()
{
  auto input = utils::get_lines();

  auto size_line_it
      = std::find_if(input.begin(), input.end(), [](auto const& line) {
          return line.size() > 1 && line[1] == '1';
        });
  auto size_line = *size_line_it;

  auto sizes    = utils::split(size_line, " ");
  auto max_size = utils::stoi(
      *std::find_if(sizes.rbegin(), sizes.rend(), [](auto const& s) {
        return !s.empty() && !std::isspace(s[0]);
      }));

  auto ret = state(max_size);

  for (auto l = input.begin(); l != size_line_it; ++l) {
    for (auto i = 1; i < l->size(); i += 4) {
      auto c = l->at(i);

      if (!std::isspace(c)) {
        ret.stacks_[(i - 1) / 4].push_back(c);
      }
    }
  }

  for (auto& s : ret.stacks_) {
    std::reverse(s.begin(), s.end());
  }

  auto ret_2 = ret;

  for (auto m_it = size_line_it + 2; m_it != input.end(); ++m_it) {
    ret.apply(move::parse(*m_it), false);
    ret_2.apply(move::parse(*m_it), true);
  }

  return {ret, ret_2};
}

int main()
{
  auto [s1, s2] = state::load();

  std::cout << s1.message() << '\n';
  std::cout << s2.message() << '\n';
}
