#include "stack.h"

error stackVerify(stack_t *stk, check flag)
{
    switch (flag)
    {
        case POINTER_CHECK:
            if (stk == NULL)
            {
                printf("A null pointer was passed\n");

                return INCORRECT_POINTER;
            }

            break;

        case ZERO_STACK_CHECK:
        {
            if (stk->data == NULL)
            {
                printf("Memory allocation error\n");

                return ALLOCATION_ERROR;
            }

            int poisonFlag = 0;
            for (size_t i = 0; i < stk->capacity; i++)
            {
                if (!compareDoubleWithDouble(stk->data[i]))
                {
                    poisonFlag = 1;
                    break;
                }
            }
            if (stk->size != 0 || stk->capacity != 0 || poisonFlag)
            {
                printf("The stack was created incorrectly\n");

                return ZERO_STACK;
            }

            break;
        }

        case ALLOCATION_CHECK:
            if(stk->data == NULL)
            {
                printf("Memory allocation error\n");

                return ALLOCATION_ERROR;
            }

            break;

        #ifdef STACK_DEBUG
            case INITIALIZATION_CHECK:
                if (stk->functionName == NULL || stk->fileName == NULL || stk->line <= 0)
                {
                    printf("Initialization did not complete correctly\n");

                    return INITIALIZATION_ERROR;
                }

                break;

        #endif

        case IS_POP_POSSIBLE:
            if (stk->size <= 0 || stk->capacity < stk->size || stk->capacity <= 0)
            {
                printf("The function cannot be called\n");

                return POP_NOT_POSSIBLE;
            }

            break;

        default:
            return INCORRECT_FLAG;

            break;
    }

    return CORRECT;
}
