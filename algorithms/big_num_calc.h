/* *****************************************************************************
Copyright: alvkeke, 2022
License: MIT
Created on 2022-03-04
***************************************************************************** */


#ifndef __ALGO_BIG_NUM_CALC_H_HH
#define __ALGO_BIG_NUM_CALC_H_HH

#include <stdint.h>
#include <stddef.h>

/**
 * get the sum of 2 big numbers. ( `num1 + num2` )
 * @param n length(bytes) of the numbers
 * @param num1 number 1
 * @param num2 number 2
 * @param ret pointer to the buffer for holding the result. one of the operate number can be pass in as a result buffer
 * @return carry
 */
int algo_big_num_add(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret);

/**
 * process increase the number by 1 ( `num++` )
 * @param n length(bytes) of the numbers
 * @param num number 1
 * @param ret pointer to the buffer for holding the result. `num` can be pass in as a result buffer
 * @return carry
 */
int algo_big_num_inc(size_t n, const uint8_t *num, uint8_t *ret);

/**
 * reserve bits of a number ( `~num` )
 * @param n length(bytes) of the numbers
 * @param num number need to be ~
 * @param ret pointer to the buffer for holding the result. `num` can be pass in as a result buffer
 */
void algo_big_num_not(size_t n, const uint8_t *num, uint8_t *ret);

/**
 * process a sub operation : ( `num1 - num2` )
 * @param n length(bytes) of numbers
 * @param num1 number 1
 * @param num2 number 2
 * @param ret pointer to the buffer for holding the result. one of the operate number can be pass in as a result buffer
 * @return carry if <= 0, other for errno
 */
int algo_big_num_sub(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret);

/**
 * compare 2 big number
 * @param n length(bytes) of numbers
 * @param num1 number 1
 * @param num2 number 2
 * @return 0 for `num1==num2`, positive for `num1>num2`, negative for `num1<num2`
 */
int algo_big_num_cmp(size_t n, const uint8_t *num1, const uint8_t *num2);

/**
 * multiple 2 number. ( `num1*num2` )
 * @param n length(bytes) of numbers
 * @param num1 number 1
 * @param num2 number 2
 * @param ret pointer to the buffer for holding the result. one of the operate number can be pass in as a result buffer
 * @return carry if <= 0, other for errno
 */
int algo_big_num_mul(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret);

/**
 * check if a number is zero
 * @param n length(bytes) of numbers
 * @param num number need to be check
 * @return 1 for yes, 0 for no
 */
int algo_big_num_is_zero(size_t n, const uint8_t *num);

/**
 * divide `num1` by `num2` ( `num1/num2` )
 * @param n length(bytes) of numbers
 * @param num1 number 1
 * @param num2 number 2
 * @param ret pointer to the buffer for holding the result. one of the operate number can be pass in as a result buffer
 * @return carry if <= 0, other for errno
 */
int algo_big_num_div(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret);

/**
 * get mod of `num1` by `num2` ( `num1%num2` )
 * @param n length(bytes) of numbers
 * @param num1 number 1
 * @param num2 number 2
 * @param ret pointer to the buffer for holding the result. one of the operate number can be pass in as a result buffer
 * @return carry if <= 0, other for errno
 */
int algo_big_num_mod(size_t n, const uint8_t *num1, const uint8_t *num2, uint8_t *ret);

/**
 * check if a big number is a prime number
 * @param n length(bytes) of numbers
 * @param num number need to be check
 * @return 0 for not, 1 for yes, negative as errno
 */
int algo_big_num_is_prime(size_t n, const uint8_t *num);

#endif //__ALGO_BIG_NUM_CALC_H_HH
