#include <intcode.h>

#include <cassert>
#include <iostream>
#include <map>
#include <string>

using loc_t = std::pair<int, int>;

class painter {
public:
  painter(std::string const&);

  void run();

  int count() const;

private:
  void paint(int col);
  int  get() const;
  void left();
  void right();
  void step();

  int comp_out_;

  int                  dir_;
  loc_t                loc_;
  ic::computer         computer_;
  std::map<loc_t, int> grid_;
};

painter::painter(std::string const& prog)
    : dir_(0)
    , comp_out_(0)
    , loc_ {0, 0}
    , computer_(
          prog, [this](auto i) { comp_out_ = i; }, true)
    , grid_ {}
{
}

void painter::paint(int col) { grid_[loc_] = col; }

int painter::get() const
{
  auto lookup = grid_.find(loc_);
  if (lookup != grid_.end()) {
    return lookup->second;
  } else {
    return 0;
  }
}

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
  while (!computer_.halted()) {
    computer_.input(get());

    computer_.run();
    int col = comp_out_;

    computer_.run();
    int turn = comp_out_;

    paint(col);

    if (turn == 0) {
      left();
    } else if (turn == 1) {
      right();
    } else {
      assert(false && "Bad turn");
    }

    step();
  }
}

int main()
{
  std::string line;
  std::getline(std::cin, line);

  auto paint = painter(line);
  paint.run();

  std::cout << paint.count() << '\n';
}
