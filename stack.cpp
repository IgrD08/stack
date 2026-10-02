#include "stack.h"

error stackInit(stack_t *stk, size_t initialCapacity
              ON_DBG(,const char *functionNameRet, const char *fileNameRet, int lineNumber))
{
    printStackVerify(stk, CORRECT);

    if (MAX - initialCapacity < initialCapacity / 2)//TODO - плохая строчка по сути
    {
        return INITIALIZATION_ERROR;
    }

    #ifdef ON_DBG
        stk->functionName = functionNameRet;
        stk->fileName = fileNameRet;
        stk->line = lineNumber;
        printStackVerify(stk, stackVerify(stk, FILE_INFO_CHECK));
    #endif

    stk->pointer = (structEnum_t*)calloc(initialCapacity + 2, sizeof(structEnum_t));

    printStackVerify(stk, stackVerify(stk, ALLOCATION_CHECK));

    stk->data = &stk->pointer[1];

    printStackVerify(stk, stackVerify(stk, ZERO_STACK_CHECK));

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

    printf("%lu\n", stk->hash);

    return CORRECT;
}

error stackPush(stack_t *stk, structEnum_t value)
{
    printStackVerify(stk, stackVerify(stk, ESSENTIAL_CHECK));
    printStackVerify(stk, stackVerify(stk, HASH_CHECK));

    if (stk->size >= stk->capacity)
    {
        recalloc(stk, stk->capacity * 2);//TODO - переименовать
    }

    printStackVerify(stk, stackVerify(stk, ALLOCATION_CHECK));

    (stk->data)[(stk->size)++] = value;
    stk->hash = recountHash(stk);

    return CORRECT;
}

structEnum_t stackPop(stack_t *stk)
{
    printStackVerify(stk, stackVerify(stk, ESSENTIAL_CHECK));
    printStackVerify(stk, stackVerify(stk, HASH_CHECK));

    if (stackVerify(stk, IS_POP_POSSIBLE) != CORRECT)
    {
        printStackVerify(stk, POP_NOT_POSSIBLE);

        return poison;
    }

    stk->size--;
    structEnum_t value = (stk->data)[stk->size];
    stk->data[stk->size] = poison;

    if (stk->size * 4 <= stk->capacity)
    {
        recalloc(stk, stk->capacity / 2);
    }

    stk->hash = recountHash(stk);

    return value;
}

error recalloc(stack_t *stk, size_t newCapacity)
{
    printStackVerify(stk, stackVerify(stk, ESSENTIAL_CHECK));

    if (newCapacity == 0) newCapacity = 1;
    structEnum_t *newStk = (structEnum_t*)realloc(stk->pointer,
                            (newCapacity + 2) * sizeof(structEnum_t));

    stk->pointer = newStk;

    printStackVerify(stk, stackVerify(stk, ALLOCATION_CHECK));

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
    printStackVerify(stk, stackVerify(stk, ESSENTIAL_CHECK));

    #ifdef STACK_DEBUG
        fprintf(fp, "stack \"stk1\" [%p] created by %s() %s :%d\n{\n",
               stk->data, stk->functionName, stk->fileName, stk->line);
    #endif

    fprintf(fp, "    capacity = %d\n    size = %d\n    data = [%p]\n       {\n",
           stk->capacity, stk->size, stk->data);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size) fprintf(fp, "            *[%u] = " SPEC "\n", i, stk->data[i]);
        else fprintf(fp, "             [%u] = " SPEC "\n", i, stk->data[i]);

        if (compareDoubleWithDouble(stk->data[i], poison)) fprintf(fp, " (POISON)\n");
        else fprintf(fp, "\n");
    }
    fprintf(fp, "       }\n");

    fprintf(fp, "}\n");

    return CORRECT;
}

bool compareDoubleWithDouble(const structEnum_t value1, const structEnum_t value2)
{//TODO - привести указатель к указателю на uint64_t
    if (isnan(value1) || isnan(value2))
    {
        if (isnan(value1) && isnan(value2))
            return 1;
        else
            return 0;
    }
    else
    {
        if (fabs(value1 - value2) < EPS)
            return 1;
    }

    return 0;
}

error stackDestroy(stack_t *stk)
{
    printStackVerify(stk, stackVerify(stk, ESSENTIAL_CHECK));

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

error fileOpening(FILE **fp)
{
    if ((*fp = fopen("errors.log", "a")) == NULL)
    {
        return FILE_ERROR;
    }

    return CORRECT;
}

error closeFile(FILE **fp)
{
    if (fclose(*fp) == EOF)
    {
        return FILE_ERROR;
    }

    return CORRECT;
}

error stackVerify(stack_t *stk, check flag)
{
    switch (flag)
    {
        if (!compareDoubleWithDouble(stk->pointer[0], canary1) ||
            !compareDoubleWithDouble(stk->pointer[stk->capacity + 1], canary2) ||
            !compareDoubleWithDouble(stk->stuctCanary1, canary3) ||
            !compareDoubleWithDouble(stk->stuctCanary2, canary4))
        {
            return CANARY_DEAD;
        }

        case ESSENTIAL_CHECK:
            if (stk == NULL)
            {
                return INCORRECT_POINTER;
            }

            if (stk->data == NULL || stk->pointer == NULL ||
                MAX - stk->size < MAX / 2 || MAX - stk->capacity < MAX / 2)
            {
                return INITIALIZATION_ERROR;
            }


            break;

        case ZERO_STACK_CHECK:
        {
            int poisonFlag = 0;
            for (size_t i = 0; i < stk->capacity; i++)
            {
                if (!compareDoubleWithDouble(stk->data[i], poison))
                {
                    poisonFlag = 1;
                    break;
                }
            }
            if (stk->size != 0 || stk->capacity != 0 || poisonFlag)
            {
                return ZERO_STACK;
            }

            break;
        }

        case ALLOCATION_CHECK:
            if(stk->pointer == NULL)
            {
                return ALLOCATION_ERROR;
            }

            break;

        #ifdef STACK_DEBUG
            case FILE_INFO_CHECK:
                if (stk->functionName == NULL || stk->fileName == NULL || stk->line <= 0)
                {
                    return FILE_INFO_ERROR;
                }

                break;
        #endif

        case IS_POP_POSSIBLE:
            if (stk->size <= 0 || stk->capacity < stk->size || stk->capacity <= 0)
            {
                return POP_NOT_POSSIBLE;
            }

            break;

        case HASH_CHECK:
            if (recountHash(stk) != stk->hash)
            {
                return HASH_ERROR;
            }

            break;

        default://assert(0)
            return INCORRECT_FLAG;

            break;
    }

    return CORRECT;
}

void printStackVerify(stack_t *stk, error flag)
{
    FILE *filepointer;
    fileOpening(&filepointer);

    if (stk == NULL)
    {
        fprintf(filepointer, "The stack was created incorrectly\n");

        abort();
    }

    if (flag)
        stackDump(stk, filepointer);

    switch (flag)
    {
        case CORRECT:

            break;

        case ZERO_STACK:

            fprintf(filepointer, "The stack was created incorrectly\n");

            abort();

            break;

        case ALLOCATION_ERROR:

            fprintf(filepointer, "Memory allocation error\n");

            break;

        case INCORRECT_POINTER:

            fprintf(filepointer, "A null pointer was passed\n");

            abort();

            break;

        case INITIALIZATION_ERROR:

            fprintf(filepointer, "An incorrectly constructed test was submitted\n");

            break;

        case INCORRECT_FLAG:

            fprintf(filepointer, "Transferred incorrect flag\n");

            abort();

            break;

        case POP_NOT_POSSIBLE:

            fprintf(filepointer, "The function cannot be called\n");

            break;

        case CANARY_DEAD:

            fprintf(filepointer, "One of canaries dead\n");

            abort();

        case FILE_INFO_ERROR:

            fprintf(filepointer, "Initialization did not complete correctly\n");

            break;

        case FILE_ERROR:

            fprintf(filepointer, "Failed to open/close file\n");

            break;

        case HASH_ERROR:

            fprintf(filepointer, "Incorrect modification of data on the stack\n");

            abort();

        default:

            break;
    }

    closeFile(&filepointer);
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
