#include "stack.h"

error stackVerify(stack_t *stk)
{
    error flag = CORRECT;

    if ((flag = essentialCheck(stk)) != CORRECT) return flag;

    if ((flag = allocationCheck(stk)) != CORRECT) return flag;

    if ((flag = hashCheck(stk)) != CORRECT) return flag;

    if ((flag = canaryCheck(stk)) != CORRECT) return flag;

    #ifdef STACK_DEBUG
        if ((flag = fileInfoCheck(stk)) != CORRECT) return flag;
    #endif

    return CORRECT;
}

error essentialCheck(stack_t *stk)
{
    if (stk == NULL)
    {
         return INCORRECT_POINTER;
    }

    if (stk->data == NULL || stk->size > MAX || stk->capacity > MAX)
    {
        return INITIALIZATION_ERROR;
    }

    return CORRECT;
}

error hashCheck(stack_t *stk)
{
    if (recountHash(stk) != stk->hash)
    {
        return HASH_ERROR;
    }

    return CORRECT;
}

error canaryCheck(stack_t *stk)
{
    if (!compareDoubleWithDouble(stk->pointer[0], canary1) ||
        !compareDoubleWithDouble(stk->pointer[stk->capacity + 1], canary2) ||
        !compareDoubleWithDouble(stk->stuctCanary1, canary3) ||
        !compareDoubleWithDouble(stk->stuctCanary2, canary4))
    {
        return CANARY_DEAD;
    }

    return CORRECT;
}

error popPossibleCheck(stack_t *stk)
{
    if (stk->size <= 0 || stk->capacity < stk->size || stk->capacity <= 0)
    {
        return POP_NOT_POSSIBLE;
    }

    return CORRECT;
}

#ifdef STACK_DEBUG
    error fileInfoCheck(stack_t *stk)
    {
        if (stk->functionName == NULL || stk->fileName == NULL || stk->line <= 0)
        {
            return FILE_INFO_ERROR;
        }

        return CORRECT;
    }
#endif

error zeroStackCheck(stack_t *stk)
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

    return CORRECT;
}

error allocationCheck(stack_t *stk)
{
    if (stk->pointer == NULL)
    {
        return ALLOCATION_ERROR;
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
