/*
 * OpenMPTL - C++ Microprocessor Template Library
 *
 * Copyright 2013 Axel Burri <axel@tty0.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define CONFIG_USE_STD_TUPLE

#include <typelist.hpp>
#include <arch/rcc.hpp>

#include <iostream>
#include <tuple>
#include <string>

using namespace mptl;
using namespace mptl::reg;

void reg_reaction::react() { }

#if 0

// helper function to print a tuple of any size
template<class Tp, std::size_t N>
struct tuple_regmask_reset_to {
  static void command(const Tp& t) 
  {
    tuple_regmask_reset_to<Tp, N-1>::command(t);
    // std::cout << "# " << std::get<N-1>(t).value << std::endl;
    std::get<N-1>(t).reset_to();
  }
};
 
template<class Tp>
struct tuple_regmask_reset_to<Tp, 1> {
  static void command(const Tp& t) 
  {
    // std::cout << std::get<0>(t).value << std::endl;
    std::get<0>(t).reset_to();
  }
};

template<class... Args>
void reset_all(const std::tuple<Args...> & t) 
{
  std::cout << "****** reset_all ******" << std::endl;
  tuple_regmask_reset_to<decltype(t), sizeof...(Args)>::command(t);
  std::cout << "****** end reset_all ******" << std::endl;
}
#endif



template<typename func_type, std::size_t N = 0, typename... Tp>
inline typename std::enable_if<N == sizeof...(Tp), void>::type
for_each(std::tuple<Tp...> & t)
{ }

template<typename func_type, std::size_t N = 0, typename... Tp>
inline typename std::enable_if<N < sizeof...(Tp), void>::type
for_each(std::tuple<Tp...> & t)
{
  using ele_type = typename std::tuple_element<N, std::tuple<Tp...> >::type;

  func_type::template command< ele_type >();
  for_each<func_type, N+1, Tp...>(t);
}


struct tuple_functor_reg_reset_to {
  template<typename list_element_type>
  static void __always_inline command(void) {
    list_element_type::reset_to();
  }
};


int main()
{
  using list = typelist<
    RCC::AHB2ENR::OTGFSEN,
    RCC::PLLCFGR::PLLSRC,
    RCC::CFGR::PPRE1::DIV4
    >;

  auto tup = list::tuple_type();

for_each<tuple_functor_reg_reset_to>(tup);

  std::cout << "====================" << std::endl;

  reset_all(tup);
}
