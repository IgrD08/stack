#include "stack.h"

error stackInit(stack_t *stk, size_t initialCapacity
              ON_DBG(,const char *functionNameRet, const char *fileNameRet, int lineNumber))
{
    STACK_VERIFY(stk, POINTER_CHECK);

    #ifdef STACK_DEBUG
        stk->functionName = functionNameRet;
        stk->fileName = fileNameRet;
        stk->line = lineNumber;
        STACK_VERIFY(stk, INITIALIZATION_CHECK);
    #endif

    stk->pointer = (structEnum_t*)calloc(initialCapacity + 2, sizeof(structEnum_t));
    stk->data = &(stk->pointer[1]);

    STACK_VERIFY(stk, ZERO_STACK_CHECK);
    STACK_VERIFY(stk, ALLOCATION_CHECK);

    stk->pointer[0] = canary1;
    stk->pointer[initialCapacity + 1] = canary2;

    stk->capacity = initialCapacity;

    for (size_t i = 0; i < initialCapacity; i++)
    {
        stk->data[i] = _POISON;
    }

    return CORRECT;
}

error stackPush(stack_t *stk, structEnum_t value)
{
    STACK_VERIFY(stk, POINTER_CHECK);

    if (stk->size >= stk->capacity)
    {
        printf("start\n");
        recalloc(stk, stk->capacity * 2);
    }

    STACK_VERIFY(stk, ALLOCATION_CHECK);

    (stk->data)[(stk->size)++] = value;

    return CORRECT;
}

structEnum_t stackPop(stack_t *stk)
{
    if (stackVerify(stk, ALLOCATION_CHECK) != CORRECT ||
        stackVerify(stk, IS_POP_POSSIBLE) != CORRECT)
    {
        return _POISON;
    }

    stk->size--;
    structEnum_t value = (stk->data)[stk->size];
    stk->data[stk->size] = _POISON;

    if (stk->size * 4 <= stk->capacity)
    {
        recalloc(stk, stk->capacity / 2);
    }

    return value;
}

error recalloc(stack_t *stk, size_t newCapacity)
{
    STACK_VERIFY(stk, POINTER_CHECK);

    if (newCapacity == 0) newCapacity = 1;
    structEnum_t *newStk = (structEnum_t*)realloc(stk->pointer, (newCapacity + 2) * sizeof(structEnum_t));

    if (newStk == 0)
    {
        return ALLOCATION_ERROR;
    }

    stk->pointer = newStk;
    stk->data = &(stk->pointer[1]);

    if (newCapacity > stk->capacity)
    {
        for (size_t i = stk->capacity; i < newCapacity; i++)
        {
            stk->data[i] = _POISON;
        }
    }

    stk->data[newCapacity] = canary2;

    stk->capacity = newCapacity;

    return CORRECT;
}

error stackDump(stack_t *stk)
{
    STACK_VERIFY(stk, POINTER_CHECK);

    #ifdef STACK_DEBUG
        printf("\033[32mstack \"stk1\" [0x%p] created by %s() %s :%d\n{\n",
               stk->data, stk->functionName, stk->fileName, stk->line);
    #endif

    printf("    capacity = %d\n    size = %d\n    data = [0x%p]\n       {\n",
           stk->capacity, stk->size, stk->data);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size) printf("            *[%u] = " SPEC, i, stk->data[i]);
        else printf("             [%u] = " SPEC, i, stk->data[i]);

        if (compareDoubleWithDouble(stk->data[i])) printf(" \033[33m(POISON)\033[32m\n");
        else printf("\n");
    }
    printf("       }\n");

    printf("}\033[0m\n");

    return CORRECT;
}

error printMenu(stack_t *stk)
{
    STACK_VERIFY(stk, POINTER_CHECK);
    bool flag = 1;
    int callNumber = 0;

    do
    {
        printf("\033[34mEnter a number from 0 to 2.\n"
               "0 - push a value\n"
               "1 - return the last value\n"
               "2 - exit the program\033[0m\n"
              );

        if (scanf("%d", &callNumber) != 1)
        {
            while (getchar() != '\n');
            continue;
        }

        switch (callNumber)
        {
            case 0:
                {
                    structEnum_t valueToPush = 0;

                    if(scanf(SPEC, &valueToPush) != 1)
                        break;
                    stackPush(stk, valueToPush);

                    #ifdef STACK_DEBUG
                        stackDump(stk);
                    #endif
                    break;
                }

            case 1:
                printf("Last value = " SPEC "\n", stackPop(stk));
                #ifdef STACK_DEBUG
                    stackDump(stk);
                #endif
                break;

            case 2:
                flag = 0;
                break;

            default:
                break;
        }

    }while(flag);

    return CORRECT;
}

bool compareDoubleWithDouble(structEnum_t value)
{
    if (isnan(_POISON))
    {
        if (isnan(value))
            return 1;
        else
            return 0;
    }
    else
    {
        if (fabs(value - _POISON) < EPS)
            return 1;
    }

    return 0;
}

error stackDestroy(stack_t *stk)
{
    STACK_VERIFY(stk, POINTER_CHECK);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = _POISON;
    }

    free(stk->data);
    stk->data = NULL;

    stk->size = 0;
    stk->capacity = 0;

    #ifdef STACK_DEBUG
        stk->fileName = 0;
        stk->functionName = 0;
        stk->line = 0;
    #endif

    return CORRECT;
}
