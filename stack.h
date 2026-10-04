typedef double structEnum_t;
#define SPEC "%lg"

#define STACK_DEBUG

#ifdef STACK_DEBUG
    #define ON_DBG(...) __VA_ARGS__
#else
    #define ON_DBG(...)
#endif

#ifdef STACK_DEBUG
    #define STACK_INIT(STK, INITIAL_CAPACITY) stackInit(STK, INITIAL_CAPACITY \
                       ON_DBG(,__func__, __FILE__, __LINE__))
#else
    #define STACK_INIT(STK, INITIAL_CAPACITY) stackInit(STK, INITIAL_CAPACITY)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

const structEnum_t poison = NAN;
const double EPS = 1e-5;
const size_t MAX = (size_t)(-1) / 2;

const structEnum_t canary1 = (const structEnum_t)0xEDA;
const structEnum_t canary2 = (const structEnum_t)0xBEDA;
const structEnum_t canary3 = (const structEnum_t)0xE1DADEDA;
const structEnum_t canary4 = (const structEnum_t)0xBEDADEDA;

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
    FILE_ERROR = -9,
    HASH_ERROR = -10
};

struct stack_t
{
    structEnum_t stuctCanary1;
    structEnum_t *data;
    structEnum_t *pointer;
    size_t size;
    size_t capacity;
    ON_DBG(const char *functionName; const char *fileName; int line;)
    uint64_t hash;
    structEnum_t stuctCanary2;
};

error stackInit(stack_t *stk, size_t initialCapacity
                ON_DBG(, const char *functionName, const char *fileName, int line));
error stackPush(stack_t *stk, double value);
structEnum_t stackPop(stack_t *stk);
error stackRecalloc(stack_t *stk, size_t newCapacity);
bool compareDoubleWithDouble(const structEnum_t value1, const structEnum_t value2);
uint64_t recountHash(stack_t *stk);
error stackDestroy(stack_t *stk);

error stackVerify(stack_t *stk);
error essentialCheck(stack_t *stk);
error hashCheck(stack_t *stk);
error canaryCheck(stack_t *stk);
error popPossibleCheck(stack_t *stk);
#ifdef STACK_DEBUG
    error fileInfoCheck(stack_t *stk);
#endif
error zeroStackCheck(stack_t *stk);
error allocationCheck(stack_t *stk);

error stackDump(stack_t *stk, FILE *fp);
error fileOpening(FILE **fp);
error closeFile(FILE **fp);
void printStackVerify(stack_t *stk, error flag);
