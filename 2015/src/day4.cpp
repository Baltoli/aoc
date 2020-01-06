#include <utils/utils.h>

int main()
{
  std::string s = "woo";
  std::cout << utils::md5_sum(s) << '\n';
}
