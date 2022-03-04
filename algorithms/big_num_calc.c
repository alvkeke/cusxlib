/* *****************************************************************************
Copyright: alvkeke, 2022
License: MIT
Created on 2022-03-04
***************************************************************************** */

#include "big_num_calc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define  MAX(a, b) (a>b?a:b)
#define  MIN(a, b) (a<b?a:b)

#include <stdio.h>

int algo_big_num_add(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret)
{

    uint16_t tmp;
    uint8_t carry = 0;
    size_t i;

    if (!n) return 0;

    for (i=0; i<n; i++)
    {
        tmp = num1[i]+num2[i] + carry;
        carry = (tmp&0xff00) >> 8;
        ret[i] = tmp&0xff;
    }

    return carry;
}

int algo_big_num_inc(size_t n, const uint8_t *num, uint8_t *ret)
{
    uint16_t tmp;
    uint8_t carry;
    size_t i;

    if (!n) return 0;

    tmp = num[0] + 1;
    ret[0] = tmp & 0xff;
    carry = (tmp&0xff) >> 8;

    for (i=1; i<n; i++)
    {
        if (!carry) break;
        tmp = num[i] + carry;
        ret[i] = tmp & 0xff;
        carry = (tmp&0xff) >> 8;
    }
    return carry;
}

void algo_big_num_not(size_t n, const uint8_t *num, uint8_t *ret)
{
    for (size_t i=0; i<n; i++)
    {
        ret[i] = ~num[i];
    }
}

int algo_big_num_sub(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret)
{
    uint8_t *num_not;
    int carry;

    num_not = (uint8_t*) malloc(n);
    if (!num_not) return -ENOMEM;

    algo_big_num_not(n, num2, num_not);
    algo_big_num_inc(n, num_not, num_not);
    carry = !algo_big_num_add(n, num1, num_not, ret);
    free(num_not);

    return carry;
}

int algo_big_num_cmp(size_t n, const uint8_t *num1, const uint8_t *num2)
{
    size_t i;
    for (i=n-1; i>=0; i--)
    {
        if (num1[i] > num2[i]) return i;
        if (num1[i] < num2[i]) return -i;
    }

    return 0;
}

void _print_bn(size_t n, const uint8_t *num)
{
    for (int i = n-1; i >=0; --i) {
        printf("%02x", num[i]);
    }
    puts("");
}

int algo_big_num_mul(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret)
{
    size_t i, j, i_ret, n_ret;
    uint16_t tmp;
    int carry;

    if (!n) return 0;

    n_ret = 2*n;
    memset(ret, 0, n_ret);

    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
        {
            tmp = num1[i] * num2[j];
            i_ret = i+j;
            carry = algo_big_num_add(2, ret+i_ret, (uint8_t *) &tmp, ret+i_ret);
            if (carry) algo_big_num_inc(n_ret-i_ret, ret+i_ret+1, ret+i_ret+1);
        }
    }

    return carry;
}
