/* *****************************************************************************
Copyright: alvkeke, 2021-2022
License: MIT
***************************************************************************** */

#include "logger.h"

unsigned int _log_level = LOG_LVL_ALL;

FILE *_log_output_fd = NULL;


const char *log_lvl_string[] = {
    "",
    "E",
    "W",
    "I",
    "D",
    "T"
};

/*
Name            FG  BG
Black           30  40
Red             31  41
Green           32  42
Yellow          33  43
Blue            34  44
Magenta         35  45
Cyan            36  46
White           37  47
Bright Black    90  100
Bright Red      91  101
Bright Green    92  102
Bright Yellow   93  103
Bright Blue     94  104
Bright Magenta  95  105
Bright Cyan     96  106
Bright White    97  107
*/


/*
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
*/


const char *log_color_string[] = {
    "\033[0m",      // DISABLE / END
    "\x1B[31m",     // ERROR    Red             31  41
    "\x1B[33m",     // WARN     Yellow          33  43
    "\x1B[32m",     // INFO     Green           32  42
    "\x1B[34m",     // DEBUG    Blue            34  44
    "\x1B[37m",     // TRACE    White           37  47
};


void logger_lvl_set(int lvl)
{
    _log_level = lvl;
}


void logger_fd_init(FILE* fd)
{
    if (!fd)
        _log_output_fd = stderr;
    else
        _log_output_fd = fd;
}

