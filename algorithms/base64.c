/* *****************************************************************************
Copyright: alvkeke, 2021-2022
License: MIT
***************************************************************************** */

#include "base64.h"
#include <stdlib.h>
#include <string.h>

static char _base64_table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int _b64_num(char c)
{
    char *x = strchr(_base64_table, c);
    return x-_base64_table;
}

char *algo_base64_encode(const char *src, int n_src, int *n_ret)
{
    char *ret;
    char a, b, c;
    int nn;
    int n_mod;

    if (n_src == 0)
    {
        *n_ret = 0;
        return NULL;
    }

    n_mod = n_src % 3;

    nn = ((n_src -1)/3 + 1) * 4;
    nn++;

    ret = (char*)malloc(nn);
    if (!ret)
    {
        *n_ret = 0;
        return NULL;
    }

    int j=0;
    for (int i=0; i<n_src; i+=3)
    {
        a = src[i];
        b = i+1>n_src? 0 : src[i+1];
        c = i+2>n_src? 0 : src[i+2];

        ret[j++] = _base64_table[a>>2];
        ret[j++] = _base64_table[(a<<4 | b>>4) & 0x3f];
        ret[j++] = _base64_table[(b<<2 | c>>6) & 0x3f];
        ret[j++] = _base64_table[c & 0x3f];
    }

    if (n_mod == 1)
    {
        ret[nn-3] = '=';
        ret[nn-2] = '=';
    }
    else if (n_mod == 2)
    {
        ret[nn-2] = '=';
    }

    ret[nn-1] = 0x00;
    *n_ret = nn;
    return ret;
}

static int _b64_check_end(const char *src, int n)
{
    int i;
    char c;
    for (i=0; i<n; i++)
    {
        c = src[i];
        if (c>='0' && c<='9') continue;
        if (c>='a' && c<='z') continue;
        if (c>='A' && c<='Z') continue;
        if (c == '+') continue;
        if (c == '/') continue;
        if (c == '=') break;
        
        return -1;
    }
    return i;
}

char *algo_base64_decode(const char *src, int n_src, int *n_ret)
{
    char *ret;
    int nn;
    int n_mod;
    int j;
    int i;

    n_src = _b64_check_end(src, n_src);
    if (n_src < 0)
    {
        *n_ret = 0;
        return NULL;
    }

    nn = (n_src/4) * 3;
    n_mod = n_src % 4;

    if (n_mod == 1)
    {
        *n_ret = 0;
        return NULL;
    }
    if (!n_mod) nn++;
    nn += n_mod;

    ret = (char*)malloc(nn);

    j=0;
    for (i=3; i<n_src; i+=4)
    {
        ret[j++] = _b64_num(src[i-3])<<2 | _b64_num(src[i-2])>>4;
        ret[j++] = _b64_num(src[i-2])<<4 | _b64_num(src[i-1])>>2;
        ret[j++] = _b64_num(src[i-1])<<6 | _b64_num(src[i]);
    }

    switch (n_mod)
    {
    case 3:
        ret[j+1] = _b64_num(src[i-2])<<4 | _b64_num(src[i-1])>>2;
    case 2:
        ret[j] = _b64_num(src[i-3])<<2 | _b64_num(src[i-2])>>4;
        break;
    default:
        break;
    }

    ret[nn-1] = 0x00;
    *n_ret = nn;
    return ret;
}


#ifdef UNIT_TEST_ALGO_BASE64
#include <stdio.h>
#include <string.h>

int main()
{
    int ret_len;
    char *src;
    char *ret;
    
    src = "123456789";
    ret = algo_base64_encode(src, strlen(src), &ret_len);
    printf("%d:\n%s\n%s\n", ret_len, ret, "MTIzNDU2Nzg5");
    free(ret);

    src = "12345678";
    ret = algo_base64_encode(src, strlen(src), &ret_len);
    printf("%d:\n%s\n%s\n", ret_len, ret, "MTIzNDU2Nzg=");
    free(ret);

    src = "1234567";
    ret = algo_base64_encode(src, strlen(src), &ret_len);
    printf("%d:\n%s\n%s\n", ret_len, ret, "MTIzNDU2Nw==");
    free(ret);

    src = "MTIzNDU2Nzg5";
    ret = algo_base64_decode(src, strlen(src), &ret_len);
    printf("%d:\n%s\n%s\n", ret_len, ret, "123456789");
    free(ret);

    src = "MTIzNDU2Nzg";
    ret = algo_base64_decode(src, strlen(src), &ret_len);
    printf("%d:\n%s\n%s\n", ret_len, ret, "12345678");
    free(ret);

    src = "MTIzNDU2Nw";
    ret = algo_base64_decode(src, strlen(src), &ret_len);
    printf("%d:\n%s\n%s\n", ret_len, ret, "1234567");
    free(ret);

    return 0;
}

#endif // UNIT_TEST_ALGO_BASE64

