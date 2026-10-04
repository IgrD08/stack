#include "stack.h"

error stackInit(stack_t *stk, size_t initialCapacity
              ON_DBG(,const char *functionNameRet, const char *fileNameRet, int lineNumber))
{
    printStackVerify(stk, CORRECT);

    if (initialCapacity  > MAX)
    {
        printStackVerify(stk, INITIALIZATION_ERROR);

        return INITIALIZATION_ERROR;
    }

    #ifdef STACK_DEBUG
        stk->functionName = functionNameRet;
        stk->fileName = fileNameRet;
        stk->line = lineNumber;
        printStackVerify(stk, fileInfoCheck(stk));
    #endif

    stk->pointer = (structEnum_t*)calloc(initialCapacity + 2, sizeof(structEnum_t));

    printStackVerify(stk, allocationCheck(stk));

    stk->data = &stk->pointer[1];

    printStackVerify(stk, zeroStackCheck(stk));

    stk->pointer[0] = canary1;
    stk->pointer[initialCapacity + 1] = canary2;
    stk->stuctCanary1 = canary3;
    stk->stuctCanary2 = canary4;

    stk->capacity = initialCapacity;

    for (size_t i = 0; i < initialCapacity; i++)
    {
        stk->data[i] = poison;
    }

    stk->hash = recountHash(stk);

    return CORRECT;
}

error stackPush(stack_t *stk, structEnum_t value)
{
    printStackVerify(stk, stackVerify(stk));

    if (stk->size >= stk->capacity)
    {
        stackRecalloc(stk, stk->capacity * 2);
    }

    (stk->data)[(stk->size)++] = value;
    stk->hash = recountHash(stk);

    printStackVerify(stk, stackVerify(stk));

    return CORRECT;
}

structEnum_t stackPop(stack_t *stk)
{
    printStackVerify(stk, stackVerify(stk));

    if (popPossibleCheck(stk) != CORRECT)
    {
        printStackVerify(stk, POP_NOT_POSSIBLE);

        return poison;
    }

    stk->size--;
    structEnum_t value = (stk->data)[stk->size];
    stk->data[stk->size] = poison;

    if (stk->size * 4 <= stk->capacity)
    {
        stackRecalloc(stk, stk->capacity / 2);
    }

    stk->hash = recountHash(stk);

    printStackVerify(stk, stackVerify(stk));

    return value;
}

error stackRecalloc(stack_t *stk, size_t newCapacity)
{
    if (newCapacity == 0) newCapacity = 1;
    structEnum_t *newStk = (structEnum_t*)realloc(stk->pointer,
                            (newCapacity + 2) * sizeof(structEnum_t));

    stk->pointer = newStk;

    stk->data = &(stk->pointer[1]);

    if (newCapacity > stk->capacity)
    {
        for (size_t i = stk->capacity; i < newCapacity; i++)
        {
            stk->data[i] = poison;
        }
    }

    stk->data[newCapacity] = canary2;
    stk->capacity = newCapacity;

    return CORRECT;
}

error stackDump(stack_t *stk, FILE *fp)
{
    #ifdef STACK_DEBUG
        fprintf(fp, "stack \"stk1\" [%p] created by %s() %s :%d\n{\n",
               stk->data, stk->functionName, stk->fileName, stk->line);
    #endif

    fprintf(fp, "    capacity = %d\n    size = %d\n    data = [%p]\n       {\n",
           stk->capacity, stk->size, stk->data);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size) fprintf(fp, "            *[%u] = " SPEC "\n", i, stk->data[i]);
        else fprintf(fp, "             [%u] = " SPEC, i, stk->data[i]);

        if (compareDoubleWithDouble(stk->data[i], poison)) fprintf(fp, " (POISON)\n");
        else fprintf(fp, "\n");
    }
    fprintf(fp, "       }\n");

    fprintf(fp, "}\n");

    return CORRECT;
}

bool compareDoubleWithDouble(const structEnum_t value1, const structEnum_t value2)
{
    if (isnan(value1) || isnan(value2))
    {
        if (isnan(value1) && isnan(value2))
            return 1;
        else
            return 0;
    }
    else
    {
        if (*(const uint64_t*)(&value1) == *(const uint64_t*)(&value2))
            return 1;
    }

    return 0;
}

error stackDestroy(stack_t *stk)
{
    printStackVerify(stk, stackVerify(stk));

    for (size_t i = 0; i < stk->capacity; i++)
    {
        stk->data[i] = poison;
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

uint64_t recountHash(stack_t *stk)
{
    uint64_t hash = 5381;

    for (uint8_t i = 0; i < (stk->capacity + 2) * sizeof(structEnum_t); i++)
    {
        hash = ((hash << 5) + hash) + ((uint8_t*)(stk->pointer))[i];
    }

    return hash;
}
