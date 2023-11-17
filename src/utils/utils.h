#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include "TSingleton.hpp"
#include "color.h"
#include "nullptr_t.hpp"
#include "types.h"
#include "const.h"

class logTimeFormat {
   public:
	static const char* accessTimeFormat;
	static const char* errorTimeFormat;
	static const char* systemTimeFormat;
	static const char* GMTtimeFormat;
	static const char* UTCtimeFormat;
	static const char* dirListFormat;
};	// namespace logTimeFormat

#endif
