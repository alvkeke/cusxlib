/* *****************************************************************************
Copyright: alvkeke, 2021-2022
License: MIT
***************************************************************************** */

#ifndef __ALGO_BASE64_H__
#define __ALGO_BASE64_H__

char *algo_base64_encode(const char *src, int n_src, int *n_ret);
char *algo_base64_decode(const char *src, int n_src, int *n_ret);

#endif // __ALGO_BASE64_H__