typedef double structEnum_t;
#define SPEC "%lg"


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
    CORRECT,
    STACK_OVERFLOW,
    ZERO_STACK,
    ALLOCATION_ERROR
};

struct struc_t
{
    structEnum_t *data;
    size_t size;
    size_t capacity;
    enum error errorEnum;
    ON_DBG(const char *functionName; const char *fileName; int line;)
};

int stackInit(struc_t *stk, size_t initialCapacity
              ON_DBG(,const char *functionName, const char *fileName, int line));
size_t stackPush(struc_t *stk, double value);
structEnum_t stackPop(struc_t *stk);
void recalloc(struc_t *stk, size_t newCapacity);
void stackDump(struc_t *stk);
void printMenu(struc_t *stk);
bool compareWithZero(structEnum_t value);
