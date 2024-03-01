#pragma once
#include "ansiescapecodes.h"
#include <iostream>
#define debugerror(print) std::cout << ANSI_COLOR_RED << "ERROR: " << print << ANSI_RESET << std::endl
#define debugwarn(print) std::cout << ANSI_COLOR_YELLOW << "WARNING: " << print << ANSI_RESET << std::endl
#define debugnote(print) std::cout << ANSI_COLOR_GREEN << "NOTE: " << print << ANSI_RESET << std::endl
