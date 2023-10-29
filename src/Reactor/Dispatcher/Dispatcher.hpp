#pragma once
#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include "Dispatcher.h"

namespace reactor {

	class Dispatcher : public utils::TSingleton<Dispatcher> {
	   private:
		SyncEventDemultiplexer* _demultiplexer;
		std::map<fd_t, AEventHandler*> _handlers;

		Dispatcher(const Dispatcher& obj);
		Dispatcher& operator=(const Dispatcher& obj);

	   public:
		Dispatcher();
		~Dispatcher();
		void registerHander(AEventHandler* handler, enum EventType type);
		void removeHander(AEventHandler* handler, enum EventType type);
		void handleEvent();
	};
}  // namespace reactor

#endif
