#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdio.h>

enum LOG_LVL {
    LOG_LVL_DISABLE = 0,
    LOG_LVL_ERROR,
    LOG_LVL_WARN,
    LOG_LVL_INFO,
    LOG_LVL_DEBUG,
    LOG_LVL_TRACE,
    LOG_LVL_END,

    LOG_LVL_ALL = (-1U)
};


extern unsigned int _log_level;
extern FILE *_log_output_fd;
extern const char *log_lvl_string[];
extern const char *log_color_string[];

/**
 * @brief set logger level, message higher then the level set will not be printed out.
 * 
 * @param lvl new logger level
 */
void logger_lvl_set(int lvl);
/* *****************************************************************************
Copyright: alvkeke, 2021-2022
License: MIT
***************************************************************************** */

/**
 * @brief init the logger fd
 * 
 * @param fd file to output the log, NULL for default: stderr
 */
void logger_fd_init(FILE* fd);

/**
 * @brief set file that log to be output.
 * 
 */
#define logger_fd_set(fd) logger_fd_init(fd)

#define PRINTF(...) if (_log_output_fd) fprintf(_log_output_fd, __VA_ARGS__)

#define LOG(LEVEL, FMT, ...) \
        do{\
            if (LEVEL < LOG_LVL_END && _log_level >= LEVEL){\
                PRINTF("%s[%s] %s(%d) - " FMT "%s", log_color_string[LEVEL], \
                        log_lvl_string[LEVEL], __FUNCTION__, __LINE__, ##__VA_ARGS__, log_color_string[0]);\
            }\
        }while(0);


#define LOG_E(...) LOG(LOG_LVL_ERROR, __VA_ARGS__)
#define LOG_W(...) LOG(LOG_LVL_WARN, __VA_ARGS__)
#define LOG_I(...) LOG(LOG_LVL_INFO, __VA_ARGS__)
#define LOG_D(...) LOG(LOG_LVL_DEBUG, __VA_ARGS__)
#define LOG_T(...) LOG(LOG_LVL_TRACE, __VA_ARGS__)

#endif  // __LOGGER_H__