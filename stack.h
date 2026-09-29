typedef double structEnum_t;
#define SPEC "%lg"

#define STACK_VERIFY(STK, FLAG)  \
if (stackVerify(STK, FLAG))      \
    return INCORRECT_POINTER;    \

#define STACK_DEBUG

#ifdef STACK_DEBUG
    #define ON_DBG(...) __VA_ARGS__
#else
    #define ON_DBG(...)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

const double EPS = 1e-5;

enum error
{
    CORRECT = 0,
    ZERO_STACK,
    ALLOCATION_ERROR,
    INCORRECT_POINTER,
    INITIALIZATION_ERROR,
    INCORRECT_FLAG,
    POP_NOT_POSSIBLE
};

enum check
{
    ZERO_STACK_CHECK = -1,
    ALLOCATION_CHECK = -2,
    CHECK_POINTER = -3,
    INITIALIZATION_CHECK = -4,
    CHECK_FLAG = -5,
    IS_POP_POSSIBLE = -6
};

struct struc_t
{
    structEnum_t *data;
    size_t size;
    size_t capacity;
    ON_DBG(const char *functionName; const char *fileName; int line;)
};

error stackInit(struc_t *stk, size_t initialCapacity
                ON_DBG(,const char *functionName, const char *fileName, int line));
error stackPush(struc_t *stk, double value);
structEnum_t stackPop(struc_t *stk);
error stackVerify(struc_t *stk, check flag);
error recalloc(struc_t *stk, size_t newCapacity);
error stackDump(struc_t *stk);
error printMenu(struc_t *stk);
bool compareWithZero(structEnum_t value);
