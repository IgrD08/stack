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
#include <math.h>

const structEnum_t poison = NAN;
const double EPS = 1e-5;
const size_t MAX = -1;
const size_t HASH = 5381;

const structEnum_t canary1 = (const structEnum_t)0xEDA;
const structEnum_t canary2 = (const structEnum_t)0xBEDA;
const structEnum_t canary3 = (const structEnum_t)0xE1DADEDA;
const structEnum_t canary4 = (const structEnum_t)0xEBA1DEDA;

enum error
{
    CORRECT = 0,
    ZERO_STACK = -1,
    ALLOCATION_ERROR = -2,
    INCORRECT_POINTER = -3,
    INITIALIZATION_ERROR = -4,
    INCORRECT_FLAG = -5,
    POP_NOT_POSSIBLE = -6,
    CANARY_DEAD = -7,
    FILE_INFO_ERROR = -8,
    FILE_ERROR = -9
};

enum check
{
    ZERO_STACK_CHECK,
    ALLOCATION_CHECK,
    ESSENTIAL_CHECK,
    IS_POP_POSSIBLE,
    FILE_INFO_CHECK
};

struct stack_t
{
    structEnum_t stuctCanary1;
    structEnum_t *data;
    structEnum_t *pointer;
    size_t size;
    size_t capacity;
    ON_DBG(const char *functionName; const char *fileName; int line;)
    structEnum_t stuctCanary2;
};

error stackInit(stack_t *stk, size_t initialCapacity
                ON_DBG(,const char *functionName, const char *fileName, int line));
error stackPush(stack_t *stk, double value);
structEnum_t stackPop(stack_t *stk);
error stackVerify(stack_t *stk, check flag);
error recalloc(stack_t *stk, size_t newCapacity);
error stackDump(stack_t *stk, FILE *fp);
bool compareDoubleWithDouble(const structEnum_t value1, const structEnum_t value2);
error stackDestroy(stack_t *stk);
error fileOpening(FILE **fp);
error closeFile(FILE **fp);
void printStackVerify(stack_t *stk, error flag);
error recount(stack_t *stk);
