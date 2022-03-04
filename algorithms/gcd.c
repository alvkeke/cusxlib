/* *****************************************************************************
Copyright: alvkeke, 2022
License: MIT
Created on 2022-03-03
***************************************************************************** */

#include "gcd.h"

#define MAX(a, b) (a>b? a : b)
#define MIN(a, b) (a<b? a : b)

unsigned int algo_gcd(unsigned int a, unsigned int b)
{

    unsigned int c;

    if (!a || !b) return 0;
    if (a == b) return a;

    c = MAX(a, b);
    b = MIN(a, b);
    a = c;

    while(1)
    {
        c = a%b;
        if (c == 0) return b;
        a = b;
        b = c;
    }
}


