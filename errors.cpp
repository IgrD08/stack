#include "stack.h"

error stackVerify(stack_t *stk)
{
    error flag = CORRECT;

    if ((flag = essentialCheck(stk)) != CORRECT) return flag;

    if ((flag = allocationCheck(stk->pointer)) != CORRECT) return flag;

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

    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery(stk, &mbi, sizeof(mbi)) == 0 ||
        mbi.State != MEM_COMMIT ||
        (mbi.Protect & PAGE_NOACCESS) ||
        (mbi.Protect & PAGE_GUARD))
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
    if (!compareValues(stk->pointer[0], canary1) ||
        !compareValues(stk->pointer[stk->capacity + 1], canary2) ||
        !compareValues(stk->stuctCanary1, canary3) ||
        !compareValues(stk->stuctCanary2, canary4))
    {
        return CANARY_DEAD;
    }

    return CORRECT;
}

error popPossibleCheck(stack_t *stk)
{
    if (stk->size == 0 || stk->capacity < stk->size || stk->capacity == 0)
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
    if (stk->size != 0 || stk->capacity != 0)
    {
        return ZERO_STACK;
    }

    return CORRECT;
}

error allocationCheck(structEnum_t *ptr)
{
    if (ptr == NULL)
    {
        return ALLOCATION_ERROR;
    }

    return CORRECT;
}

returnStatus printStackVerify(stack_t *stk, error flag)
{
    FILE *filepointer = NULL;

    size_t fatalFlag = 0;

    if (fileOpening(&filepointer) != CORRECT)
    {
        return ERROR_STATUS;
    }

    if (stk == NULL)
    {
        fprintf(filepointer, "ERROR: The stack was created incorrectly\n");

        if (closeFile(&filepointer) != CORRECT)
        {
            fatalFlag = 1;
        }
    }

    if (!fatalFlag)
    {
        switch (flag)
        {
            case CORRECT:

                fatalFlag = 0;

                break;

            case ZERO_STACK:

                fprintf(filepointer, "ERROR: The stack was created incorrectly\n");

                fatalFlag = 1;

                break;

            case ALLOCATION_ERROR:

                fprintf(filepointer, "ERROR: Memory allocation error\n");

                fatalFlag = 2;

                break;

            case INCORRECT_POINTER:

                fprintf(filepointer, "ERROR: A null pointer was passed\n");

                fatalFlag = 1;

                break;

            case INITIALIZATION_ERROR:

                fprintf(filepointer, "ERROR: An incorrectly constructed test was submitted\n");

                fatalFlag = 1;

                break;

            case POP_NOT_POSSIBLE:

                fprintf(filepointer, "ERROR: The function cannot be called\n");

                fatalFlag = 2;

                break;

            case CANARY_DEAD:

                fprintf(filepointer, "ERROR: One of canaries dead\n");

                fatalFlag = 1;

            case FILE_INFO_ERROR:

                fprintf(filepointer, "ERROR: Initialization did not complete correctly\n");

                fatalFlag = 2;

                break;

            case FILE_ERROR:

                fprintf(filepointer, "ERROR: Failed to open/close file\n");

                fatalFlag = 2;

                break;

            case HASH_ERROR:

                fprintf(filepointer, "ERROR: Incorrect modification of data on the stack\n");

                fatalFlag = 1;

            default:

                break;
        }
    }

    if (fatalFlag == 1)
    {
        return ERROR_STATUS;
    }

    if (fatalFlag == 2)
    {
        stackDump(stk, filepointer);

        return ERROR_STATUS;
    }

    if (closeFile(&filepointer) != CORRECT)
    {
        return ERROR_STATUS;
    }

    return NORMAL_STATUS;
}

error fileOpening(FILE **fp)
{
    if ((*fp = fopen(ERRORS_FILE, "a")) == NULL)
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
