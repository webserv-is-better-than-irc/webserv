#include "HttpMessage.hpp"

HttpMessage::HttpMessage() : _startLine(3), _headers(), _body(), _errorCode(0) {}

HttpMessage::HttpMessage(const HttpMessage& obj) {
	*this = obj;
}

HttpMessage::~HttpMessage() {}

HttpMessage& HttpMessage::operator=(const HttpMessage& obj) {
	if (this != &obj) {
		this->_startLine = obj._startLine;
		this->_headers = obj._headers;
		this->_body = obj._body;
	}
	return *this;
}

std::string HttpMessage::combineHeaders(void) {
	std::string httpRequest;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); ++it) {
		httpRequest += it->first + ": " + it->second + "\r\n";
	}
	return httpRequest;
}

std::string HttpMessage::getRawStr(void) {
	const std::string startLine = utils::join(_startLine, " ") + "\r\n";
	const std::string headers = this->combineHeaders() + "\r\n";

	return startLine + headers;
}

void HttpMessage::setErrorCode(const int errorCode) {
	this->_errorCode = errorCode;
}

int HttpMessage::getErrorCode(void) const {
	return this->_errorCode;
}

std::map<std::string, std::string>& HttpMessage::getHeaders(void) {
	return this->_headers;
}

void HttpMessage::setStartLine(const std::vector<std::string> startLine) {
	this->_startLine[0] = startLine[0];	 // method | Http-version
	this->_startLine[1] = startLine[1];	 // request-target | status code
	this->_startLine[2] = startLine[2];	 // HTTP-version | reason-phrase
}

enum HttpMethods HttpMessage::getMethod(void) const {
	const std::string method = this->_startLine[0];

	if (method == "GET")
		return GET;
	if (method == "POST")
		return POST;
	if (method == "DELETE")
		return DELETE;
	if (method == "PUT")
		return PUT;
	return UNKNOWN;
}

std::string HttpMessage::getRequestTarget(void) const {
	return this->_startLine[1];
}

void HttpMessage::setHeaders(const std::map<std::string, std::string>& headers) {
	this->_headers = headers;
}

void HttpMessage::setBody(const std::string& body) {
	this->_body = body;
}

void HttpMessage::reset() {
	this->_startLine[0].clear();
	this->_startLine[1].clear();
	this->_startLine[2].clear();
	this->_headers.clear();
	this->_body.clear();
}
