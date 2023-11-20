#include "HttpSession.hpp"

HttpSession::HttpSession() : _sessionStore(SessionStore::getInstance()) {}

HttpSession::~HttpSession() {}

sessionId_t HttpSession::createSession(const std::time_t& expiredTime) {
	const sessionId_t sessionId = utils::generateRandomString();
	SessionData sessionData(expiredTime);
	this->_sessionStore->setSession(sessionId, sessionData);
	return sessionId;
}

SessionData& HttpSession::getSessionData(const sessionId_t& sessionId) {
	return this->_sessionStore->getSession(sessionId);
}