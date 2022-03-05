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
    carry = (tmp&0xff00) >> 8;

    for (i=1; i<n; i++)
    {
        if (!carry) break;
        tmp = num[i] + carry;
        ret[i] = tmp & 0xff;
        carry = (tmp&0xff00) >> 8;
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

static int algo_big_num_sub_priv(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret, uint8_t *tmp_buf)
{
    algo_big_num_not(n, num2, tmp_buf);
    algo_big_num_inc(n, tmp_buf, tmp_buf);
    return !algo_big_num_add(n, num1, tmp_buf, ret);
}

int algo_big_num_sub(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret)
{
    uint8_t *num_not;
    int carry;

    num_not = (uint8_t*) malloc(n);
    if (!num_not) return -ENOMEM;

    carry = algo_big_num_sub_priv(n, num1, num2, ret, num_not);
    free(num_not);

    return carry;
}

int algo_big_num_cmp(size_t n, const uint8_t *num1, const uint8_t *num2)
{
    return memcmp(num1, num2, n);
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
            if (carry && i_ret+2 < n_ret) algo_big_num_inc(n_ret-i_ret, ret+i_ret+2, ret+i_ret+2);
        }
    }

    return carry;
}

int algo_big_num_is_zero(size_t n, const uint8_t *num)
{
    for (int i = 0; i < n; ++i) {
        if (num[i]) return 0;
    }

    return 1;
}

int algo_big_num_div(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret)
{
    uint8_t *tmp_buf;
    uint8_t *sub_ret;
    int carry;

    if (!n) return -EINVAL;
    if (algo_big_num_is_zero(n, num2)) return -EINVAL;

    tmp_buf = (uint8_t*) malloc(n);
    if (!tmp_buf) return -ENOMEM;
    sub_ret = (uint8_t*) malloc(n);
    if (!sub_ret)
    {
        free(tmp_buf);
        return -ENOMEM;
    }

    memset(ret, 0, n);
    if (algo_big_num_is_zero(n, num1)) return 0;
    memcpy(sub_ret, num1, n);

    while(1){
        carry = algo_big_num_sub_priv(n, sub_ret, num2, sub_ret, tmp_buf);
        if (carry) break;
        algo_big_num_inc(n, ret, ret);
    }

    free(tmp_buf);
    free(sub_ret);
    return 0;
}

int algo_big_num_mod(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret)
{
    uint8_t *tmp_buf;

    if (!n) return -EINVAL;
    if (algo_big_num_is_zero(n, num2)) return -EINVAL;

    tmp_buf = (uint8_t*) malloc(n);
    if (!tmp_buf) return -ENOMEM;

    memset(ret, 0, n);
    if (algo_big_num_is_zero(n, num1)) return 0;
    memcpy(ret, num1, n);

    while(1){
        if (algo_big_num_cmp(n, ret, num2) < 0) break;
        algo_big_num_sub_priv(n, ret, num2, ret, tmp_buf);
    }

    free(tmp_buf);
    return 0;
}

int algo_big_num_is_prime(size_t n, const uint8_t *num)
{
    uint8_t *itr;
    uint8_t *buf;
    int ret;

    itr = (uint8_t*) malloc(n);
    if (!itr) return -ENOMEM;
    buf = (uint8_t*) malloc(n);
    if (!buf)
    {
        free(itr);
        return -ENOMEM;
    }

    memset(itr + 1, 0, n - 1);
    itr[0] = 2;

    while(algo_big_num_cmp(n, itr, num) < 0)    // while(itr<num)
    {
        ret = algo_big_num_mod(n, num, itr, buf);   // buf = num % itr;
        if (ret < 0) goto algo_big_num_is_prime_exit;
        if (algo_big_num_is_zero(n, buf))   // if (buf == 0);
        {
            ret = 0;
            goto algo_big_num_is_prime_exit;
        }
        algo_big_num_inc(n, itr, itr);  // itr++
    }

    ret = 1;
algo_big_num_is_prime_exit:
    free(itr);
    free(buf);
    return ret;
}

#ifdef UNIT_TEST_ALGO_BIG_NUM

#include <stdio.h>
#include "prime_num.h"

int main()
{
    uint32_t a, b, c;
    uint8_t *x_ret = (uint8_t *) &c;
    int carry;

    // ============================= add test ============================================================
    a = 0x12345678;
    b = 0x87654321;
    carry = algo_big_num_add(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %08x = %08x, carry=%d\n\n", a, b, a+b, a, b, c, carry);

    a = 0x87654321;
    b = 0x12345678;
    carry = algo_big_num_add(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %08x = %08x, carry=%d\n\n", a, b, a+b, a, b, c, carry);

    a = 0xffffffff;
    b = 1;
    carry = algo_big_num_add(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %08x = %08x, carry=%d\n\n", a, b, a+b, a, b, c, carry);

    a = 1;
    b = 0xffffffff;
    carry = algo_big_num_add(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %08x = %08x, carry=%d\n\n", a, b, a+b, a, b, c, carry);

    a = 0xffff0000;
    b = 0x00010000;
    carry = algo_big_num_add(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %08x = %08x, carry=%d\n\n", a, b, a+b, a, b, c, carry);

    a = 0xffffffff;
    b = 0xffffffff;
    carry = algo_big_num_add(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %08x = %08x, carry=%d\n\n", a, b, a+b, a, b, c, carry);

    a = 0xfeff01;
    b = 0xfe0100;
    carry = algo_big_num_add(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %08x = %08x, carry=%d\n\n", a, b, a+b, a, b, c, carry);

    a = 0xfeff01;
    b = 0xfe01;
    c = a;
    carry = algo_big_num_add(2, x_ret+1, (const uint8_t *) &b, x_ret+1);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %06x00 = %08x, carry=%d\n\n", a, b<<8, a+(b<<8), a, b, c, carry);

    a = 0xfeff;
    b = 0xfe01;
    c = 0;
    carry = algo_big_num_add(sizeof(a)/2, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x + %08x = %08x\nbig num: %08x + %08x = %08x, carry=%d\n\n", a, b, a+b, a, b, c, carry);

    // =================================== sub test =======================================================
    a = 1;
    b = 1;
    carry = algo_big_num_sub(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x - %08x = %08x\nbig num: %08x - %08x = %08x, carry=%d\n\n", a, b, a-b, a, b, c, carry);

    a = 2;
    b = 1;
    carry = algo_big_num_sub(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x - %08x = %08x\nbig num: %08x - %08x = %08x, carry=%d\n\n", a, b, a-b, a, b, c, carry);

    a = 1;
    b = 2;
    carry = algo_big_num_sub(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x - %08x = %08x\nbig num: %08x - %08x = %08x, carry=%d\n\n", a, b, a-b, a, b, c, carry);

    a = 0xffffffff;
    b = 0xffffffff;
    carry = algo_big_num_sub(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x - %08x = %08x\nbig num: %08x - %08x = %08x, carry=%d\n\n", a, b, a-b, a, b, c, carry);

    a = 1;
    b = 0xffffffff;
    carry = algo_big_num_sub(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x - %08x = %08x\nbig num: %08x - %08x = %08x, carry=%d\n\n", a, b, a-b, a, b, c, carry);

    // ==================================== inc test =======================================================
    c = 0;
    x_ret = (uint8_t *) &c;
    printf("big_num_inc test from %08x: ", c);
    for (int i=0; i<5; i++)
    {
        carry = algo_big_num_inc(sizeof(c), x_ret, x_ret);
        printf("%08x[%d] ", c, carry);
    }
    puts("");

    c = 0xfd;
    x_ret = (uint8_t *) &c;
    printf("big_num_inc test from %08x: ", c);
    for (int i=0; i<5; i++)
    {
        carry = algo_big_num_inc(sizeof(c), x_ret, x_ret);
        printf("%08x[%d] ", c, carry);
    }
    puts("");

    c = 0xfffffffd;
    x_ret = (uint8_t *) &c;
    printf("big_num_inc test from %08x: ", c);
    for (int i=0; i<5; i++)
    {
        carry = algo_big_num_inc(sizeof(c), x_ret, x_ret);
        printf("%08x[%d] ", c, carry);
    }
    puts("\n");

    // ===================================== is_zero test =======================================================
    c = 0;
    carry = algo_big_num_is_zero(sizeof(c), x_ret);
    printf("%08x %s zero\n", c, carry? "is" : "is not");

    c = 1;
    carry = algo_big_num_is_zero(sizeof(c), x_ret);
    printf("%08x %s zero\n", c, carry? "is" : "is not");

    c = -1;
    carry = algo_big_num_is_zero(sizeof(c), x_ret);
    printf("%08x %s zero\n\n", c, carry? "is" : "is not");

    // ==================================== cmp test =========================================================
    a = 1;
    b = 1;
    carry = algo_big_num_cmp(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b);
    printf("[%d] %08x %s %08x\n", carry, a, carry>0 ? ">" : (carry==0 ? "==" : "<"), b);

    a = 0;
    b = 0;
    carry = algo_big_num_cmp(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b);
    printf("[%d] %08x %s %08x\n", carry, a, carry>0 ? ">" : (carry==0 ? "==" : "<"), b);

    a = 0xffffffff;
    b = 0xffffffff;
    carry = algo_big_num_cmp(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b);
    printf("[%d] %08x %s %08x\n", carry, a, carry>0 ? ">" : (carry==0 ? "==" : "<"), b);

    a = 2;
    b = 1;
    carry = algo_big_num_cmp(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b);
    printf("[%d] %08x %s %08x\n", carry, a, carry>0 ? ">" : (carry==0 ? "==" : "<"), b);

    a = 1;
    b = 2;
    carry = algo_big_num_cmp(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b);
    printf("[%d] %08x %s %08x\n", carry, a, carry>0 ? ">" : (carry==0 ? "==" : "<"), b);

    a = 0xffffffff;
    b = 1;
    carry = algo_big_num_cmp(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b);
    printf("[%d] %08x %s %08x\n", carry, a, carry>0 ? ">" : (carry==0 ? "==" : "<"), b);

    a = 1;
    b = 0xffffffff;
    carry = algo_big_num_cmp(sizeof(a), (const uint8_t *) &a, (const uint8_t *) &b);
    printf("[%d] %08x %s %08x\n\n", carry, a, carry>0 ? ">" : (carry==0 ? "==" : "<"), b);

    // ========================================= not test ==================================================
    a = 0xf0f0f0f0;
    algo_big_num_not(sizeof(a), (const uint8_t *) &a, x_ret);
    printf("~%08x == %08x\n", a, c);

    // ======================================== mul test ==================================================
    a = 0x1234;
    b = 0x3456;
    carry = algo_big_num_mul(2, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x * %08x = %08x\nbig num: %08x * %08x = %08x, carry=%d\n\n", a, b, a*b, a, b, c, carry);

    a = 0;
    b = 0x3456;
    carry = algo_big_num_mul(2, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x * %08x = %08x\nbig num: %08x * %08x = %08x, carry=%d\n\n", a, b, a*b, a, b, c, carry);

    a = 0x3456;
    b = 0;
    carry = algo_big_num_mul(2, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x * %08x = %08x\nbig num: %08x * %08x = %08x, carry=%d\n\n", a, b, a*b, a, b, c, carry);

    a = 0xffff;
    b = 0xffff;
    carry = algo_big_num_mul(2, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x * %08x = %08x\nbig num: %08x * %08x = %08x, carry=%d\n\n", a, b, a*b, a, b, c, carry);

    a = 1;
    b = 0xfafa;
    carry = algo_big_num_mul(2, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x * %08x = %08x\nbig num: %08x * %08x = %08x, carry=%d\n\n", a, b, a*b, a, b, c, carry);


    // =========================================== div test =========================================================
    a = 12;
    b = 1;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = %08x\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a/b, a, b, c, carry);

    a = 12;
    b = 2;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = %08x\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a/b, a, b, c, carry);

    a = 12;
    b = 3;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = %08x\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a/b, a, b, c, carry);

    a = 12;
    b = 4;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = %08x\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a/b, a, b, c, carry);

    a = 12;
    b = 5;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = %08x\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a/b, a, b, c, carry);

    a = 12;
    b = 6;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = %08x\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a/b, a, b, c, carry);

    a = 12;
    b = 7;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = %08x\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a/b, a, b, c, carry);

    a = 12;
    b = 13;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = %08x\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a/b, a, b, c, carry);

    a = 12;
    b = 0;
    carry = algo_big_num_div(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x / %08x = NIL\nbig num: %08x / %08x = %08x, carry=%d\n\n", a, b, a, b, c, carry);

    // =========================================== mod test =========================================================
    a = 12;
    b = 1;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = %08x\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a%b, a, b, c, carry);

    a = 12;
    b = 2;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = %08x\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a%b, a, b, c, carry);

    a = 12;
    b = 3;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = %08x\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a%b, a, b, c, carry);

    a = 12;
    b = 4;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = %08x\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a%b, a, b, c, carry);

    a = 12;
    b = 5;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = %08x\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a%b, a, b, c, carry);

    a = 12;
    b = 6;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = %08x\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a%b, a, b, c, carry);

    a = 12;
    b = 7;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = %08x\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a%b, a, b, c, carry);

    a = 12;
    b = 13;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = %08x\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a%b, a, b, c, carry);

    a = 12;
    b = 0;
    carry = algo_big_num_mod(4, (const uint8_t *) &a, (const uint8_t *) &b, x_ret);
    printf("origin:  %08x mod %08x = NIL\nbig num: %08x mod %08x = %08x, carry=%d\n\n", a, b, a, b, c, carry);

    puts("====== is prime num test ========");
    for(uint32_t i=2; i<11; i++)
    {
        carry = algo_big_num_is_prime(sizeof(i), (uint8_t*)&i);
        printf("[%02u] %d:%d, ", i, algo_is_num_prime(i), carry);
    }
    puts("");

    for(uint32_t i=11; i<20; i++)
    {
        carry = algo_big_num_is_prime(sizeof(i), (uint8_t*)&i);
        printf("[%02u] %d:%d, ", i, algo_is_num_prime(i), carry);
    }
    puts("");
    puts("====== is prime num test ========\n");

#include <sys/time.h>
    struct timeval t11, t12, t21, t22;

#define ITR_TIMES 1
    a = 0x43528761;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_is_num_prime(a);
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_is_prime(sizeof(a), (uint8_t*)&a);
    gettimeofday(&t22, NULL);

    printf("is prime[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

#define ITR_TIMES 10
    a = 0x43528761;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_is_num_prime(a);
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_is_prime(sizeof(a), (uint8_t*)&a);
    gettimeofday(&t22, NULL);

    printf("is prime[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

#define ITR_TIMES 100
    a = 0x43528761;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_is_num_prime(a);
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_is_prime(sizeof(a), (uint8_t*)&a);
    gettimeofday(&t22, NULL);

    printf("is prime[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

#define ITR_TIMES 1000
    a = 0x43528761;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_is_num_prime(a);
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_is_prime(sizeof(a), (uint8_t*)&a);
    gettimeofday(&t22, NULL);

    printf("is prime[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

#define ITR_TIMES 10000
    a = 0x43528761;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_is_num_prime(a);
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_is_prime(sizeof(a), (uint8_t*)&a);
    gettimeofday(&t22, NULL);

    printf("is prime[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

//#define ITR_TIMES 100000
//    a = 0x43528761;
//    gettimeofday(&t11, NULL);
//    for (int i=0; i<ITR_TIMES; i++) algo_is_num_prime(a);
//    gettimeofday(&t12, NULL);
//
//    gettimeofday(&t21, NULL);
//    for (int i=0; i<ITR_TIMES; i++) algo_big_num_is_prime(sizeof(a), (uint8_t*)&a);
//    gettimeofday(&t22, NULL);
//
//    printf("is prime[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
//           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
//           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
//           );


#define ITR_TIMES 10
    a = 0x43528761;
    b = 0x32123123;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) c = a+b;
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_add(4, (uint8_t*)&a,(uint8_t*) &b,(uint8_t*) &c);
    gettimeofday(&t22, NULL);

    printf("add[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

#define ITR_TIMES 100
    a = 0x43528761;
    b = 0x32123123;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) c = a+b;
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_add(4, (uint8_t*)&a,(uint8_t*) &b,(uint8_t*) &c);
    gettimeofday(&t22, NULL);

    printf("add[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

#define ITR_TIMES 1000
    a = 0x43528761;
    b = 0x32123123;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) c = a+b;
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_add(4, (uint8_t*)&a,(uint8_t*) &b,(uint8_t*) &c);
    gettimeofday(&t22, NULL);

    printf("add[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

#define ITR_TIMES 10000
    a = 0x43528761;
    b = 0x32123123;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) c = a+b;
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_add(4, (uint8_t*)&a,(uint8_t*) &b,(uint8_t*) &c);
    gettimeofday(&t22, NULL);

    printf("add[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

#define ITR_TIMES 100000
    a = 0x43528761;
    b = 0x32123123;
    gettimeofday(&t11, NULL);
    for (int i=0; i<ITR_TIMES; i++) c = a+b;
    gettimeofday(&t12, NULL);

    gettimeofday(&t21, NULL);
    for (int i=0; i<ITR_TIMES; i++) algo_big_num_add(4, (uint8_t*)&a,(uint8_t*) &b,(uint8_t*) &c);
    gettimeofday(&t22, NULL);

    printf("add[%d times] : origin: %ld:%ld,  bignum: %ld:%ld\n", ITR_TIMES,
           t12.tv_sec-t11.tv_sec, t12.tv_usec-t11.tv_usec,
           t22.tv_sec-t21.tv_sec, t22.tv_usec-t21.tv_usec
           );

    return 0;
}

#endif
