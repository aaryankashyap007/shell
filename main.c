#include "defs.h"

bool exit_bool = true;

int main()
{
    system("clear");
    start();
    while (1)
    {
        print();
        if(!exit_bool)
        {
            system("clear");
            break;
        }
    }
    return 0;
}