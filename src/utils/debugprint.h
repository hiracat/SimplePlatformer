#pragma once
#include "../utils/ansiescapecodes.h"
#include "iostream"

#ifndef NDEBUG

#define debugerror(print)                                                                                                                  \
    std::cerr << ANSI_COLOR_RED << "ERROR: " << print << ANSI_RESET << std::endl;                                                          \
    abort();

#define debugwarn(print) std::cerr << ANSI_COLOR_YELLOW << "WARNING: " << print << ANSI_RESET << std::endl
#define debugnote(print) std::cerr << ANSI_COLOR_GREEN << "NOTE: " << print << ANSI_RESET << std::endl
#define debugunknown(print) std::cerr << ANSI_COLOR_BLUE << "UNKNOWN: " << print << ANSI_RESET << std::endl

#else
#define debugerror(print)                                                                                                                  \
    std::cerr << ANSI_COLOR_RED << "ERROR: " << print << ANSI_RESET << std::endl;                                                          \
    abort();

#define debugwarn(print)
#define debugnote(print)
#define debugunknown(print)
#endif

#ifdef DEBUG_DATA
#define debugdata(print) std::cerr << ANSI_RESET << "DATA: " << print << std::endl
#else
#define debugdata(print)
#endif
