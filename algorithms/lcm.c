/* *****************************************************************************
Copyright: alvkeke, 2022
License: MIT
Created on 2022-03-03
***************************************************************************** */

#include "lcm.h"
#include "gcd.h"

unsigned int algo_lcm(unsigned int a, unsigned int b)
{

    unsigned int xx;

    if (!a || !b) return 0;
    if (a == b) return a;

    xx = algo_gcd(a, b);

    return a*b/xx;
}
