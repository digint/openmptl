#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#define __always_inline  inline __attribute__((always_inline))
#define __naked                 __attribute__((naked)) __attribute__((noinline))
#define __noreturn              __attribute__((noreturn))

#endif // COMPILER_H_INCLUDED
