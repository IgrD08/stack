#include "stack.h"

int main()
{
    struc_t stk1 = {};
    size_t initialCapacity = 1;

    stackInit(&stk1, initialCapacity
              ON_DBG(,__func__, __FILE__, __LINE__));

    printMenu(&stk1);

    //TODO - наладить возвращаемое значение

    return 0;
}
