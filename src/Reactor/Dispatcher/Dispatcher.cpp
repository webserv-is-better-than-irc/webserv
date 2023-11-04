#include "Dispatcher.hpp"

namespace reactor {

	Dispatcher::Dispatcher() : _demultiplexer(SyncEventDemultiplexer::getInstance()) {}

	Dispatcher::~Dispatcher() {}

	template <class Factory>
	void Dispatcher::registerIOHandler(sharedData_t sharedData) {
		const handle_t handle = sharedData.get()->fd;
		Factory factory;
		u::shared_ptr<AEventHandler> handler = factory.createHandler(sharedData);

		this->_ioHandlers[handle].push_back(handler);
		this->_handlerIndices[handler] = this->_ioHandlers[handle].size() - 1;
		this->_demultiplexer->requestEvent(handler.get(), sharedData.get()->type);
	}

	template <class Factory>
	void Dispatcher::registerExeHandler(sharedData_t sharedData, ...) {
		const handle_t handle = sharedData->fd;
		Factory factory;
		va_list args;
		va_start(args, sharedData);
		u::shared_ptr<AEventHandler> handler = factory.createHandler(sharedData, args);
		va_end(args);

		this->_exeHandlers[handle].push_back(handler);
		this->_handlerIndices[handler] = this->_exeHandlers[handle].size() - 1;
	}

	/*IOhandler 하나만 Dispatcher, kevent에서 삭제합니다.*/
	void Dispatcher::removeIOHandler(fd_t fd, enum EventType type) {

		if (this->_ioHandlers.find(fd) != this->_ioHandlers.end()) {
			std::vector<u::shared_ptr<AEventHandler> > handlers = this->_ioHandlers.find(fd)->second;
			u::shared_ptr<AEventHandler> handler;

			for (std::vector<u::shared_ptr<AEventHandler> >::iterator it = handlers.begin(); it != handlers.end();
				 ++it) {
				if (it->get()->getType() == type)
					handler = *it;
			}

			size_t index = this->_handlerIndices[handler];

			if (index != this->_ioHandlers[fd].size() - 1) {
				std::swap(this->_ioHandlers[fd][index], this->_ioHandlers[fd].back());
				this->_handlerIndices[this->_ioHandlers[fd][index]] = index;
			}

			this->_demultiplexer->unRequestEvent(this->_ioHandlers[fd].back().get(), type);
			this->_ioHandlers[fd].pop_back();
			this->_handlerIndices.erase(handler);
		}
	}
	/*Exehandler 하나만 Dispatcher에서 삭제합니다.*/
	void Dispatcher::removeExeHandler(u::shared_ptr<AEventHandler> handler) {
		const handle_t handle = handler->getHandle();

		if (this->_exeHandlers.find(handle) != this->_exeHandlers.end()) {
			const size_t index = this->_handlerIndices[handler];

			if (index != this->_exeHandlers[handle].size() - 1) {
				std::swap(this->_exeHandlers[handle][index], this->_exeHandlers[handle].back());
				this->_handlerIndices[this->_exeHandlers[handle][index]] = index;
			}

			this->_exeHandlers[handle].pop_back();
			this->_handlerIndices.erase(handler);
		}
	}

	void Dispatcher::addFdToClose(fd_t fd) {
		this->_fdsToClose.insert(fd);
	}

	void Dispatcher::removeFdToClose(fd_t fd) {
		this->_fdsToClose.erase(fd);
	}

	bool Dispatcher::isFdMarkedToClose(fd_t fd) const {
		return (this->_fdsToClose.find(fd) != this->_fdsToClose.end());
	}
	/*연결이 종료 되어야할 clientFd들을 연결 종료합니다.(IOhandler만 모두 삭제합니다.)*/
	void Dispatcher::closePendingFds() {
		for (std::set<fd_t>::iterator it = this->_fdsToClose.begin(); it != this->_fdsToClose.end(); ++it) {
			if (this->_ioHandlers.find(*it) != this->_ioHandlers.end()) {
				this->_demultiplexer->unRequestAllEvent(*it);
				ServerManager::getInstance()->eraseClient(*it);

				std::vector<u::shared_ptr<AEventHandler> > handlersToErase = this->_ioHandlers[*it];
				for (std::vector<u::shared_ptr<AEventHandler> >::iterator handlerIt = handlersToErase.begin();
					 handlerIt != handlersToErase.end(); ++handlerIt)
					this->_handlerIndices.erase(*handlerIt);

				this->_ioHandlers.erase(*it);
				std::cout << *it << " : was closed\n";
			}
		}
		this->_fdsToClose.clear();
	}

	void Dispatcher::exeHandlerexe() {
		for (std::map<fd_t, std::vector<u::shared_ptr<AEventHandler> > >::iterator it = this->_exeHandlers.begin();
			 it != this->_exeHandlers.end(); ++it) {
			for (std::vector<u::shared_ptr<AEventHandler> >::iterator evnetIt = it->second.begin();
				 evnetIt != it->second.end(); ++evnetIt) {
				evnetIt->get()->handleEvent();
			}
		}
	}

	void Dispatcher::handleEvent(void) {
		if (this->_fdsToClose.size() != 0)
			this->closePendingFds();
		_demultiplexer->waitEvents();

		// 벡터 순회하면서 executeHandler-> handleEvent실행.
	}
}  // namespace reactor
