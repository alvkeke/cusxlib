/* *****************************************************************************
Copyright: alvkeke, 2022
License: MIT
Created on 2022-03-12
***************************************************************************** */

#ifndef __LIB_MYSYSTEM_H__
#define __LIB_MYSYSTEM_H__

/**
 * run an outside command with redirected stdin/stdout
 * @param fout receive the fd of the stdout, NULL for not used (`close` must be called if not NULL)
 * @param fin receive the fd of the stdin, NULL for not used (`close` must be called if not NULL)
 * @param cmd command need to run (see `execvp`)
 * @param ... parameters (see `execvp`)
 * @return 0 for success, others for errno.
 */
int mysystem(int *fout, int *fin, const char *cmd, ...);

#endif //__LIB_MYSYSTEM_H__
