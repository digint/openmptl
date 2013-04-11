#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

/* Function attributes */
#ifndef __always_inline
# define __always_inline  inline __attribute__((always_inline))
#endif

#define __naked                 __attribute__((naked)) __attribute__((noinline))
#define __noreturn              __attribute__((noreturn))

/* Variable attributes */
#define __used                  __attribute__((used))

#endif // COMPILER_H_INCLUDED
