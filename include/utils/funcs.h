#include <random>

namespace eng::utils {

inline auto random_range(int a, int b) -> int {
  return std::rand() % (b - a + 1) + a;
}

}
