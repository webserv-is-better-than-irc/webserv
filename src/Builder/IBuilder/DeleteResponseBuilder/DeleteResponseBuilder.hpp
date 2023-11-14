#pragma once
#ifndef DELETE_RESPONSE_BUILDER_HPP
#define DELETE_RESPONSE_BUILDER_HPP

#include "DeleteResponseBuilder.h"

class DeleteResponseBuilder : public IBuilder<reactor::sharedData_t> {
   private:
	reactor::sharedData_t _sharedData;
	const request_t _request;
	const utils::shared_ptr<ServerConfig> _serverConfig;
	const utils::shared_ptr<LocationConfig> _locationConfig;  // may be needed?

	HttpMessage _response;

   public:
	DeleteResponseBuilder(reactor::sharedData_t sharedData, request_t request,
						  const utils::shared_ptr<ServerConfig>& serverConfig,
						  const utils::shared_ptr<LocationConfig>& locationConfig);
	~DeleteResponseBuilder();

	void deleteFile();
	virtual enum AsyncState getReadState() const { return this->_sharedData.get()->getState(); }
	virtual void setReadState(enum AsyncState state) { this->_sharedData.get()->setState(state); }
	virtual reactor::sharedData_t getProduct();
	virtual void setStartLine();
	virtual void setHeader();
	virtual bool setBody();
	virtual void reset();
	virtual bool build();
	virtual void prepare();
};

#endif