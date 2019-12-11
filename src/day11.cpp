#include <intcode.h>

#include <map>
#include <string>

using loc_t = std::pair<int, int>;

class painter {
public:
  painter(std::string const&);

  void paint(loc_t loc, int col);
  int  get(loc_t loc) const;

  int count() const;

private:
  int comp_out_;

  loc_t                loc_;
  ic::computer         computer_;
  std::map<loc_t, int> grid_;
};

painter::painter(std::string const& prog)
    : comp_out_(0)
    , loc_ {0, 0}
    , computer_(prog, [this](auto i) { comp_out_ = i; })
    , grid_ {}
{
}

void painter::paint(loc_t loc, int col) { grid_[loc] = col; }

int painter::get(loc_t loc) const
{
  auto lookup = grid_.find(loc);
  if (lookup != grid_.end()) {
    return lookup->second;
  } else {
    return 0;
  }
}

int painter::count() const { return grid_.size(); }

int main() {}
