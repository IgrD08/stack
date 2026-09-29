#include "stack.h"

error stackInit(struc_t *stk, size_t initialCapacity
              ON_DBG(,const char *functionNameRet, const char *fileNameRet, int lineNumber))
{
     if (stk == NULL)
        return INCORRECT_POINTER;

    stk->data = (structEnum_t*)calloc(initialCapacity, sizeof(structEnum_t));

    STACK_VERIFY(stk, ALLOCATION_CHECK);

    stk->capacity = initialCapacity;

    STACK_VERIFY(stk, ZERO_STACK_CHECK);

    #ifdef STACK_DEBUG
        stk->functionName = functionNameRet;
        stk->fileName = fileNameRet;
        stk->line = lineNumber;
        STACK_VERIFY(stk, INITIALIZATION_CHECK);
    #endif

    return CORRECT;
}

error stackPush(struc_t *stk, structEnum_t value)
{
    STACK_VERIFY(stk, ALLOCATION_CHECK);

    if (stk->size >= stk->capacity)
    {
        recalloc(stk, stk->capacity * 2);
    }

    STACK_VERIFY(stk, ALLOCATION_CHECK);

    (stk->data)[(stk->size)++] = value;

    return CORRECT;
}

structEnum_t stackPop(struc_t *stk)
{
    if (stackVerify(stk, ALLOCATION_CHECK) != CORRECT ||
        stackVerify(stk, IS_POP_POSSIBLE) != CORRECT)
    {
        return 0;//TODO - return Poison
    }

    stk->size--;
    structEnum_t value = (stk->data)[stk->size];
    stk->data[stk->size] = 0;

    if (stk->size * 4 <= stk->capacity)
    {
        recalloc(stk, stk->capacity / 2);
    }

    return value;
}

error stackVerify(struc_t *stk, check flag)
{
    if (stk == NULL)
    {
        printf("A null pointer was passed\n");

        return INCORRECT_POINTER;
    }

    if (flag == ZERO_STACK_CHECK)
    {
        if (stk->data == NULL)
        {
            printf("Memory allocation error\n");

            return ALLOCATION_ERROR;
        }

        int poisonFlag = 0;
        for (size_t i = 0; i < stk->capacity; i++)
        {
            if (!compareWithZero(stk->data[i]))
            {
                poisonFlag = 1;
                break;
            }
        }
        if (stk->size > stk->capacity || poisonFlag)
        {
            printf("The stack was created incorrectly\n");

            return ZERO_STACK;
        }
        else
            return CORRECT;
    }
    else if (flag == ALLOCATION_CHECK)
    {
        if(stk->data == NULL)
        {
            printf("Memory allocation error\n");

            return ALLOCATION_ERROR;
        }
        else
            return CORRECT;
    }
    #ifdef STACK_DEBUG
        else if (flag == INITIALIZATION_CHECK)
        {
            if (stk->functionName == NULL || stk->fileName == NULL)
            {
                printf("Initialization did not complete correctly\n");

                return INITIALIZATION_ERROR;
            }
            else
                return CORRECT;
        }
    #endif
    else if (flag == IS_POP_POSSIBLE)
    {
        if (stk->size <= 0 || stk->capacity < stk->size || stk->capacity <= 0)
        {
            printf("The function cannot be called\n");

            return POP_NOT_POSSIBLE;
        }
        else
            return CORRECT;
    }
    else
        return INCORRECT_FLAG;
}

error recalloc(struc_t *stk, size_t newCapacity)
{
    STACK_VERIFY(stk, ALLOCATION_CHECK);

    if (newCapacity == 0) newCapacity = 1;

    structEnum_t *newStk = (structEnum_t*)realloc(stk->data, newCapacity * sizeof(structEnum_t));

    if (newStk == NULL)
    {
        return ALLOCATION_ERROR;
    }

    if (newCapacity > stk->capacity)
    {
        for (size_t i = stk->capacity; i < newCapacity; i++)
        {
            newStk[i] = 0;
        }
    }

    stk->capacity = newCapacity;
    stk->data = newStk;

    return CORRECT;
}

error stackDump(struc_t *stk)
{
    STACK_VERIFY(stk, ALLOCATION_CHECK);

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

        if (compareWithZero(stk->data[i])) printf(" \033[33m(POISON)\033[32m\n");
        else printf("\n");
    }
    printf("       }\n");

    printf("}\033[0m\n");

    return CORRECT;
}

error printMenu(struc_t *stk)
{
    STACK_VERIFY(stk, ALLOCATION_CHECK);

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

bool compareWithZero(structEnum_t value)
{
    if (fabs(value) < EPS) return 1;

    return 0;
}
