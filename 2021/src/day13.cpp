#include <utils/ctre.h>
#include <utils/utils.h>

#include <cassert>
#include <iostream>
#include <set>
#include <utility>

using namespace ctre::literals;

enum axis { X, Y };

struct fold {
  axis ax;
  int  coord;

  fold(std::string const& in)
  {
    auto m = ctre::match<R"(fold along ([xy])=(\d+))">.match(in);

    auto axis = std::string(m.get<1>());
    auto c    = std::string(m.get<2>());

    ax    = axis[0] == 'x' ? axis::X : axis::Y;
    coord = utils::stoi(c);
  }
};

class chart {
public:
  using dots_t = std::set<std::pair<int, int>>;

  chart() = default;

  void add(std::string const& line)
  {
    auto m = ctre::match<R"((\d+),(\d+))">(line);
    dots_.insert(
        {utils::stoi(m.get<1>().str()), utils::stoi(m.get<2>().str())});
  }

  void apply(fold const& f)
  {
    auto to_delete = dots_t {};
    auto to_insert = dots_t {};

    for (auto const& d : dots_) {
      if (f.ax == axis::X) {
        if (d.first > f.coord) {
          auto diff = std::abs(d.first - f.coord);
          to_insert.insert({f.coord - diff, d.second});
          to_delete.insert(d);
        }
      } else {
        if (d.second > f.coord) {
          auto diff = std::abs(d.second - f.coord);
          to_insert.insert({d.first, f.coord - diff});
          to_delete.insert(d);
        }
      }
    }

    for (auto const& d : to_delete) {
      dots_.erase(d);
    }

    for (auto const& d : to_insert) {
      dots_.insert(d);
    }
  }

  int count() const { return dots_.size(); }

  void dump()
  {
    auto max_x
        = std::max_element(
              dots_.begin(), dots_.end(),
              [](auto const& a, auto const& b) { return a.first < b.first; })
              ->first;

    auto max_y
        = std::max_element(
              dots_.begin(), dots_.end(),
              [](auto const& a, auto const& b) { return a.second < b.second; })
              ->second;

    for (auto y = 0; y <= max_y; ++y) {
      for (auto x = 0; x <= max_x; ++x) {
        if (dots_.contains({x, y})) {
          std::cout << '#';
        } else {
          std::cout << ' ';
        }
      }
      std::cout << '\n';
    }
  }

private:
  dots_t dots_ = {};
};

std::pair<chart, std::vector<fold>> load()
{
  auto c  = chart();
  auto fs = std::vector<fold> {};

  bool points = true;
  utils::for_each_line([&](auto const& line) {
    if (line.empty()) {
      points = false;
    } else if (points) {
      c.add(line);
    } else {
      fs.emplace_back(line);
    }
  });

  return {c, fs};
}

int part_1(chart c, fold const& f)
{
  c.apply(f);
  return c.count();
}

void part_2(chart c, std::vector<fold> const& fs)
{
  for (auto const& f : fs) {
    c.apply(f);
  }

  c.dump();
}

int main()
{
  auto [chart, folds] = load();

  std::cout << part_1(chart, folds[0]) << '\n';
  part_2(chart, folds);
}
