#include <intcode.h>

#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <string>

using loc_t = std::pair<int, int>;

class painter {
public:
  painter(std::string const&, int start);

  void run();
  void render() const;

  int count() const;

private:
  void paint(int col);
  int  get() const;
  int  get(loc_t) const;
  void left();
  void right();
  void step();

  int comp_out_;

  int                  start_;
  int                  dir_;
  loc_t                loc_;
  ic::computer         computer_;
  std::map<loc_t, int> grid_;
};

painter::painter(std::string const& prog, int start)
    : start_(start)
    , dir_(0)
    , comp_out_(0)
    , loc_ {0, 0}
    , computer_(prog)
    , grid_ {}
{
}

void painter::paint(int col) { grid_[loc_] = col; }

int painter::get(loc_t loc) const
{
  auto lookup = grid_.find(loc);
  if (lookup != grid_.end()) {
    return lookup->second;
  } else {
    return 0;
  }
}

int painter::get() const { return get(loc_); }

void painter::left() { dir_ = (dir_ + 3) % 4; }

void painter::right() { dir_ = (dir_ + 1) % 4; }

void painter::step()
{
  switch (dir_) {
  case 0:
    loc_.second--;
    break;
  case 1:
    loc_.first++;
    break;
  case 2:
    loc_.second++;
    break;
  case 3:
    loc_.first--;
    break;
  default:
    assert(false && "Bad step");
  }
}

int painter::count() const { return grid_.size(); }

void painter::run()
{
  int inp = start_;
  while (true) {
    computer_.input(inp);

    auto col  = computer_.run();
    auto turn = computer_.run();

    if (computer_.halted()) {
      break;
    }

    paint(col);

    if (turn == 0) {
      left();
    } else if (turn == 1) {
      right();
    } else {
      assert(false && "Bad turn");
    }

    step();

    inp = get();
  };
}

void painter::render() const
{
  // minX maxX minY maxY
  auto bounds = std::array {0, 0, 0, 0};

  for (auto [loc, c] : grid_) {
    if (loc.first < bounds[0]) {
      bounds[0] = loc.first;
    }

    if (loc.first > bounds[1]) {
      bounds[1] = loc.first;
    }

    if (loc.second < bounds[2]) {
      bounds[2] = loc.second;
    }

    if (loc.second > bounds[3]) {
      bounds[3] = loc.second;
    }
  }

  for (int row = bounds[2]; row <= bounds[3]; ++row) {
    for (int col = bounds[0]; col <= bounds[1]; ++col) {
      if (get({col, row}) == 1) {
        std::cout << "█";
      } else {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  auto paint = painter(line, 0);
  paint.run();
  std::cout << paint.count() << '\n';

  auto p2 = painter(line, 1);
  p2.run();
  p2.render();
}
