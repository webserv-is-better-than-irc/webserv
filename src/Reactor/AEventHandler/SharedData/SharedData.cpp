#include "SharedData.hpp"

namespace reactor {
	SharedData::SharedData(const fd_t fd, const enum EventType type, std::vector<char> buffer)
		: _fd(fd), _type(type), _buffer(buffer), _state(PENDING), _readByte(0){};

	SharedData::SharedData(const SharedData& obj)
		: _fd(obj._fd), _type(obj._type), _buffer(obj._buffer), _state(obj._state){};

	SharedData::~SharedData(){};

	SharedData& SharedData::operator=(const SharedData& obj) {
		if (this != &obj) {
			const_cast<fd_t&>(this->_fd) = obj._fd;
			const_cast<enum EventType&>(this->_type) = obj._type;
			this->_buffer = obj._buffer;
			this->_state = obj._state;
		}
		return *this;
	};

}  // namespace reactor