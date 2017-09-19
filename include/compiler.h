#ifndef COMPILER_H_INCLUDED
#define COMPILER_H_INCLUDED

#define __weak                   __attribute__((weak))

/* Function attributes */
#ifndef __always_inline
# define __always_inline  inline __attribute__((always_inline))
#endif

// NOTE: While non-ASM statement in naked function is not supported,
// it works fine in our projects (arm). Use with care!
#ifdef CONFIG_CLANG
# define __naked                __attribute__((noinline))
#else
# define __naked                __attribute__((naked)) __attribute__((noinline))
#endif // CONFIG_CLANG

#define __noreturn              __attribute__((noreturn))

/* Variable attributes */
#ifndef __used
# define __used                  __attribute__((used))
#endif

#endif // COMPILER_H_INCLUDED
