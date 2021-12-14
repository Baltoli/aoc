#include <utils/utils.h>

#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>

long error_value(char error)
{
  static std::unordered_map<char, long> table
      = {{')', 3}, {']', 57}, {'}', 1197}, {'>', 25137}};
  return table.at(error);
}

long value(char complete)
{
  static std::unordered_map<char, long> table
      = {{')', 1}, {']', 2}, {'}', 3}, {'>', 4}};
  return table.at(complete);
}

bool opens(char c)
{
  static std::unordered_set<char> data = {'(', '[', '{', '<'};
  return data.contains(c);
}

bool closes(char c)
{
  static std::unordered_set<char> data = {')', ']', '}', '>'};
  return data.contains(c);
}

char opener(char closer)
{
  static std::unordered_map<char, char> table
      = {{')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}};
  return table.at(closer);
}

char closer(char opener)
{
  static std::unordered_map<char, char> table
      = {{'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}};
  return table.at(opener);
}

std::pair<long, long> compute(std::vector<std::string> const& in)
{
  auto sum         = 0;
  auto completions = std::vector<long> {};

  for (auto const& line : in) {
    auto complete = true;

    auto stack = std::stack<char> {};

    for (auto c : line) {
      if (opens(c)) {
        stack.push(c);
      } else if (closes(c)) {
        if (stack.top() == opener(c)) {
          stack.pop();
        } else {
          sum += error_value(c);
          complete = false;
          break;
        }
      }
    }

    if (complete) {
      auto score = 0L;
      while (!stack.empty()) {
        auto c = stack.top();
        stack.pop();

        score *= 5;
        score += value(closer(c));
      }

      completions.push_back(score);
    }
  }

  std::sort(completions.begin(), completions.end());
  return {sum, completions.at(completions.size() / 2)};
}

int main()
{
  auto in               = utils::get_lines();
  auto [part_1, part_2] = compute(in);

  std::cout << part_1 << '\n';
  std::cout << part_2 << '\n';
}
