#include "LoginFilter.h"
#include <iostream>
struct FilterDebug {
    FilterDebug() { std::cout << "LoginFilter loaded!" << std::endl; }
} filterDebugInstance;
