#include "ClientResponseHandler.hpp"

namespace reactor {

	ClientResponseHandler::ClientResponseHandler(sharedData_t& sharedData, va_list args)
		: AExeHandler(sharedData),
		  _request(*va_arg(args, request_t*)),
		  _serverConfig(ServerManager::getInstance()->getServerConfig(this->getHandle())),
		  _locationConfig(_serverConfig->getLocationConfig(_request->second.getRequestTarget())),
		  _director(this->chooseBuilder()),
		  _registered(false) {
		va_end(args);
	}

	ClientResponseHandler::~ClientResponseHandler() {}

	void ClientResponseHandler::handleEvent() {
		if (this->removeHandlerIfNecessary())
			return;
		if (this->_registered == false) {
			Dispatcher::getInstance()->registerIOHandler<ClientWriteHandlerFactory>(this->_sharedData);
			this->_registered = true;
		}
		try {
			if (this->getBuffer().empty() && this->_director.getBuilderReadState() == RESOLVE) {
				Dispatcher::getInstance()->removeIOHandler(this->getHandle(), this->getType());
				Dispatcher::getInstance()->removeExeHandler(this);
				return;
			}
			if (this->_director.buildProduct() == false)  // may be throw ErrorReponseBuilder
				return;
		} catch (const utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			this->_director.setBuilder(e);
		}
	}

	utils::shared_ptr<IBuilder<sharedData_t> > ClientResponseHandler::chooseBuilder() {
		try {
			if (this->_request->first == HTTP_ERROR)
				throw utils::shared_ptr<IBuilder<sharedData_t> >(
					new ErrorResponseBuilder(this->_request->second.getErrorCode(), this->_sharedData,
											 this->_serverConfig, this->_locationConfig));
			std::vector<enum HttpMethods> methods = this->_serverConfig->getDirectives(LIMIT_EXCEPT).asMedVec();

			if (std::find(methods.begin(), methods.end(), this->_request->second.getMethod()) == methods.end())
				throw utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
					METHOD_NOT_ALLOWED, this->_sharedData, this->_serverConfig, this->_locationConfig));
			switch (this->_request->second.getMethod()) {
				case GET:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new GetResponseBuilder(
						this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case POST:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new PostResponseBuilder(
						this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case DELETE:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new DeleteResponseBuilder(
						this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case PUT:
					// return utils::shared_ptr<IBuilder<sharedData_t> >(new PutResponseBuilder(
					// 	this->_sharedData, this->_request, this->_serverConfig, this->_locationConfig));
				case UNKNOWN:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
						BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
				default:
					return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
						BAD_REQUEST, this->_sharedData, this->_serverConfig, this->_locationConfig));
					break;
			}

		} catch (utils::shared_ptr<IBuilder<sharedData_t> >& e) {
			return e;
		} catch (...) {
			return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
				INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
		}
		return utils::shared_ptr<IBuilder<sharedData_t> >(new ErrorResponseBuilder(
			INTERNAL_SERVER_ERROR, this->_sharedData, this->_serverConfig, this->_locationConfig));
	}
}  // namespace reactor
