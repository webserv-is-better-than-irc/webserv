#pragma once
#ifndef AEVENTHANDLER_H
#define AEVENTHANDLER_H

#include <sys/socket.h>
#include "enum.h"
#include "shared_ptr.hpp"
#include "types.h"

struct sharedData {
	int fd;
	std::vector<char> buffer;
	enum AsyncState state;

	sharedData(const int fd, std::vector<char> buffer) : fd(fd), buffer(buffer), state(PENDING){};
};

typedef utils::shared_ptr<struct sharedData> sharedData_t;

#endif
