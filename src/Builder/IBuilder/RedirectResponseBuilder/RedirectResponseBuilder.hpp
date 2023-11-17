#pragma once
#ifndef REDIRECTRESPONSEBUILDER_HPP
#define REDIRECTRESPONSEBUILDER_HPP

#include "RedirectResponseBuilder.h"

class RedirectResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	const unsigned int _statusCode;
	const std::string _path;
	reactor::sharedData_t _sharedData;
	const request_t _request;
	const utils::shared_ptr<ServerConfig> _serverConfig;
	// const utils::shared_ptr<LocationConfig> _locationConfig;
	reactor::sharedData_t _readSharedData;
	HttpMessage _response;

   public:
	RedirectResponseBuilder(const unsigned int statusCode, const std::string& path, reactor::sharedData_t sharedData,
							request_t request, const utils::shared_ptr<ServerConfig>& serverConfig);
	~RedirectResponseBuilder();
	virtual enum AsyncState getReadState() const { return this->_readSharedData->getState(); }
	virtual void setReadState(enum AsyncState state) { this->_readSharedData->setState(state); }
	virtual reactor::sharedData_t getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual bool build();
	virtual void prepare();
};

#endif
