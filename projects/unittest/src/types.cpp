#include <cstdint>

int main()
{
  // see: <http://en.cppreference.com/w/cpp/types/integer>
  static_assert(sizeof(uint_fast16_t) == 4, "sizeof(uint_fast16_t) != 4");
  static_assert(sizeof(uintmax_t) == 8,     "sizeof(uintmax_t) != 8");
  static_assert(sizeof(uintptr_t) == 4,     "sizeof(uintptr_t) != 4"); /* unsigned integer type capable of holding a pointer */
  static_assert(UINT32_MAX == 0xffffffff,   "UINT32_MAX != 0xffffffff");
}
