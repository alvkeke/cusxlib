/* *****************************************************************************
Copyright: alvkeke, 2022
License: MIT
Created on 2022-03-03
***************************************************************************** */

#include "prime_num.h"

int algo_is_num_prime(unsigned int num)
{
    for (unsigned int i = 2; i<num; i++)
    {
        if (num %i == 0) return 0;
    }
    return 1;
}



