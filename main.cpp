#include "stack.h"

returnStatus printMenu(stack_t *stk);

int main()
{
    stack_t stk1 = {};
    size_t initialCapacity = 10;

    if (STACK_INIT(&stk1, initialCapacity) != NORMAL_STATUS)
    {
        printf("Invalid values passed\n");

        return 1;
    }

    //memset(&stk1, -1, sizeof(stk1));

    printMenu(&stk1);

    stackDestroy(&stk1);

    return 0;
}

returnStatus printMenu(stack_t *stk)
{
    if (printStackVerify(stk, stackVerify(stk)) == ERROR_STATUS)
    {
        return ERROR_STATUS;
    }

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

                    if (scanf(SPEC, &valueToPush) != 1)
                        break;
                    stackPush(stk, valueToPush);

                    break;
                }

            case 1:
                printf("Last value = " SPEC "\n", stackPop(stk));

                break;

            case 2:
                flag = 0;

                break;

            default:

                break;
        }

    }while(flag);

    return NORMAL_STATUS;
}


