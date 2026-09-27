#include "stack.h"

int stackInit(struc_t *stk, size_t initialCapacity
              ON_DBG(,const char *functionNameRet, const char *fileNameRet, int lineNumber))
{
    //TODO - проверка нулевая ли структура

    stk->data = (structEnum_t*)calloc(initialCapacity, sizeof(structEnum_t));

    stk->capacity = initialCapacity;

    #ifdef STACK_DEBUG
        stk->functionName = functionNameRet;
        stk->fileName = fileNameRet;
        stk->line = lineNumber;
    #endif

    return 0;
}

void stackVerify(struc_t *stk)
{
    if (isnan(stk)) 




    return;
}

size_t stackPush(struc_t *stk, structEnum_t value)
{
    if (stk->size >= stk->capacity)
    {
        recalloc(stk, stk->capacity * 2);
    }

    (stk->data)[(stk->size)++] = value;

    return stk->size;
}

structEnum_t stackPop(struc_t *stk)
{
    structEnum_t value = (stk->data)[stk->size];
    stk->data[--(stk->size)] = 0;

    if (stk->size * 2 <= stk->capacity)
    {
        recalloc(stk, stk->capacity / 2);
    }

    return value;
}

void recalloc(struc_t *stk, size_t newCapacity)
{
    if (newCapacity == 0) newCapacity = 1;

    structEnum_t *newStk = (structEnum_t*)realloc(stk->data, newCapacity * sizeof(structEnum_t));

    if (newStk == NULL)
    {
        stk->errorEnum = ALLOCATION_ERROR;

        return;
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

    return;
}//TODO - доделать recalloc: добавить прыжок

void stackDump(struc_t *stk)
{
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
}

void printMenu(struc_t *stk)
{
    bool flag = 1;
    int callNumber = 0;

    do
    {
        printf("\033[34mEnter a number from 0 to 2.\n"
               "0 - push a value\n"
               "1 - return the last value\n"
               "2 - exit the program\033[0m\n"
              );

        scanf("%d", &callNumber);

        switch (callNumber)
        {
            case 0:
                {
                    structEnum_t valueToPush = 0;

                    if(scanf(SPEC, &valueToPush) != 1) break;
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
}

bool compareWithZero(structEnum_t value)
{
    if (fabs(value) < EPS) return 1;

    return 0;
}
