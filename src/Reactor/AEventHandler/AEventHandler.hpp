#pragma once
#ifndef AEVENTHANDLER_HPP
#define AEVENTHANDLER_HPP

#include "AEventHandler.h"

namespace reactor {

	class AEventHandler {
	   private:
	   protected:
		const handle_t _fd;
		const utils::shared_ptr<AccessLogger> _accessLogger;
		const utils::shared_ptr<ErrorLogger> _errorLogger;
		utils::shared_ptr<std::vector<char> > _buf;

	   public:
		AEventHandler(const handle_t fd, const utils::shared_ptr<AccessLogger>& accessLogger,
					  const utils::shared_ptr<ErrorLogger>& errorLogger)
			: _fd(fd), _accessLogger(accessLogger), _errorLogger(errorLogger), _buf(new std::vector<char>){};
		AEventHandler(const handle_t fd, const utils::shared_ptr<AccessLogger>& accessLogger,
					  const utils::shared_ptr<ErrorLogger>& errorLogger, utils::shared_ptr<std::vector<char> > buf)
			: _fd(fd), _accessLogger(accessLogger), _errorLogger(errorLogger), _buf(buf){};
		virtual ~AEventHandler(){};
		virtual handle_t getHandle() const { return this->_fd; };
		virtual void handleEvent() = 0;
	};

}  // namespace reactor

#endif