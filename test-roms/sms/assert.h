
#ifndef __ASSERT_H
#define __ASSERT_H

#ifdef __CONSOLE_H
/*If available use VDP console*/
#include "console.h"
#define ASSERT_PRINT(s) con_put(s)


#else
/*If not, use SDSC*/
#include "sdsc.h"
#define ASSERT_PRINT(s) sdsc_puts(s)
#endif

/*I love you stackoverflow*/
#define xstr(a) str(a)
#define str(a) #a

#define ASSERT_INNER(EXP, EXP_STR, LINE, FILE_STR) {\
    if(!(EXP)){                               \
        ASSERT_PRINT(                         \
            "Assertion failed!\n"             \
            FILE_STR ":"                      \
            LINE                              \
            ": "                              \
            EXP_STR                           \
            );                                \
        while(1){}                            \
    }                                         \
}

#ifndef __FILENAME__
#define __FILENAME__ __FILE__
#endif

#ifdef WITH_ASSERT
#define assert(EXP) ASSERT_INNER(EXP, #EXP, xstr(__LINE__), __FILENAME__)
#else
#define assert(EXP)
#endif

#endif