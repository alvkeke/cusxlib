/* *****************************************************************************
Copyright: alvkeke, 2022
License: MIT
Created on 2022-03-12
***************************************************************************** */

#include "mysystem.h"

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>

#define PREAD(a) a[0]
#define PWRITE(a) a[1]

#define PCLOSE(a) do {close(a[0]); close(a[1]);}while(0)


int mysystem(int *fout, int *fin, const char *cmd, ...)
{

    int ret;
    int pcin[2];
    int pcout[2];
    va_list va;
    size_t num_va = 1;
    char **argv;


    va_start(va, cmd);
    while (va_arg(va, char*)) num_va++;
    va_end(va);

    argv = (char**)malloc(num_va * sizeof(char*));
    if (!argv) return ENOMEM;

    va_start(va, cmd);
    for(int i=0; i<num_va; i++) argv[i] = va_arg(va, char*);
    va_end(va);

    ret = pipe(pcin);
    if (ret) return errno;

    ret = pipe(pcout);
    if (ret)
    {
        PCLOSE(pcin);
        return errno;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
        PCLOSE(pcin);
        PCLOSE(pcout);
        return errno;
    }

    if (pid == 0)
    {
        close(PWRITE(pcin));
        close(PREAD(pcout));

        dup2(PREAD(pcin), STDIN_FILENO);
        dup2(PWRITE(pcout), STDOUT_FILENO);

        execvp(cmd, argv);

        close(PWRITE(pcout));
        close(PREAD(pcin));
        _exit(0);
    }

    close(PWRITE(pcout));
    close(PREAD(pcin));

    if (fout)
        *fout = PREAD(pcout);
    else
        close(PREAD(pcout));

    if (fin)
        *fin = PWRITE(pcin);
    else
        close(PWRITE(pcin));

    return 0;
}

