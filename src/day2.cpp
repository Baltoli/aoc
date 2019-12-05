#include <cassert>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> load_intcode() {
  std::vector<int> ret{};

  for (std::string line; std::getline(std::cin, line);) {
    ret.push_back(std::stoi(line));
  };

  return ret;
}

std::vector<int> interpret(std::vector<int> code) {
  auto ptr = code.begin();

  while (*ptr != 99) {
    switch (*ptr) {
      case 1:
        code[ptr[3]] = code[ptr[1]] + code[ptr[2]];
        break;
      case 2:
        code[ptr[3]] = code[ptr[1]] * code[ptr[2]];
        break;
      default:
        assert(false && "Invalid opcode");
    }

    ptr += 4;
  }

  return code;
}

void tests() {
  auto test = [](std::vector<int> a, std::vector<int> b) {
    assert((interpret(a) == b));
  };

  test({1, 0, 0, 0, 99}, {2, 0, 0, 0, 99});
  test({2, 3, 0, 3, 99}, {2, 3, 0, 6, 99});
  test({2, 4, 4, 5, 99, 0}, {2, 4, 4, 5, 99, 9801});
  test({1, 1, 1, 4, 99, 5, 6, 0, 99}, {30, 1, 1, 4, 2, 5, 6, 0, 99});
}

int main() {
  tests();

  auto prog = load_intcode();
  auto initial_state = prog;

  prog[1] = 12;
  prog[2] = 2;

  auto result = interpret(prog);
  std::cout << result[0] << '\n';

  for (int noun = 0; noun < 100; ++noun) {
    for (int verb = 0; verb < 100; ++verb) {
      auto test_prog = initial_state;
      test_prog[1] = noun;
      test_prog[2] = verb;

      if (interpret(test_prog)[0] == 19690720) {
        std::cout << (noun * 100 + verb) << '\n';
      }
    }
  }
}
