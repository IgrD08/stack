typedef double structEnum_t;
#define SPEC "%lg"

#define STACK_VERIFY(STK, FLAG)  \
if (stackVerify(STK, FLAG))      \
    return INCORRECT_POINTER;    \

#define _POISON NAN

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

const structEnum_t canary1 = (const structEnum_t)0xEDA;
const structEnum_t canary2 = (const structEnum_t)0xBEDA;

enum error
{
    CORRECT = 0,
    ZERO_STACK = -1,
    ALLOCATION_ERROR = -2,
    INCORRECT_POINTER = -3,
    INITIALIZATION_ERROR = -4,
    INCORRECT_FLAG = -5,
    POP_NOT_POSSIBLE = -6
};

enum check
{
    ZERO_STACK_CHECK,
    ALLOCATION_CHECK,
    INITIALIZATION_CHECK,
    POINTER_CHECK,
    IS_POP_POSSIBLE
};

struct stack_t
{
    structEnum_t *data;
    structEnum_t *pointer;
    size_t size;
    size_t capacity;
    ON_DBG(const char *functionName; const char *fileName; int line;)
};

error stackInit(stack_t *stk, size_t initialCapacity
                ON_DBG(,const char *functionName, const char *fileName, int line));
error stackPush(stack_t *stk, double value);
structEnum_t stackPop(stack_t *stk);
error stackVerify(stack_t *stk, check flag);
error recalloc(stack_t *stk, size_t newCapacity);
error stackDump(stack_t *stk);
error printMenu(stack_t *stk);
bool compareDoubleWithDouble(structEnum_t value);
error stackDestroy(stack_t *stk);
