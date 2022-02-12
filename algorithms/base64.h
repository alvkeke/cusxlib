/* *****************************************************************************
Copyright: alvkeke, 2021-2022
License: MIT
***************************************************************************** */

#ifndef __ALGO_BASE64_H__
#define __ALGO_BASE64_H__

/**
 * @brief encode data to base64, `free` need to be called for releasing result string.
 * 
 * @param src source data
 * @param n_src length of source data
 * @param n_ret length of result string
 * @return result string, NULL for failed.
 */
char *algo_base64_encode(const unsigned char *src, int n_src, int *n_ret);

/**
 * @brief decode an encoded-base64-string, `free` need to be called for release result string.
 * 
 * @param src encoded string
 * @param n_src length of the encoded string.
 * @param n_ret length of the original data.
 * @return original data
 */
char *algo_base64_decode(const char *src, int n_src, int *n_ret);

#endif // __ALGO_BASE64_H__