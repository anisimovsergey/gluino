// Copyright Sergey Anisimov 2016-2017
// MIT License
//
// Gluino
// https://github.com/anisimovsergey/gluino

#ifndef MESSAGING_QUEUE_RESOURCE_CONTROLLER_HPP
#define MESSAGING_QUEUE_RESOURCE_CONTROLLER_HPP

#include "QueueController.hpp"
#include "ResourceRequestHandler.hpp"

#include <vector>

namespace Messaging {

class IMessageQueue;

class QueueResourceController {
  TYPE_PTRS(QueueResourceController)
  public:
    QueueResourceController(std::string resource, QueueController& queueController);

    Core::StatusResult::Unique sendEvent(std::string eventType);
    Core::StatusResult::Unique sendEvent(std::string eventType, Core::IEntity::Unique content);

    void addOnRequest(std::string requestType, std::function<Core::IEntity::Unique()> onRequest) {
      handlers.push_back(ResourceRequestHandlerVoid::makeUnique(requestType, onRequest));
    }

    template<class T>
    void addOnRequest(std::string requestType, std::function<Core::IEntity::Unique(const T&)> onRequest) {
      handlers.push_back(ResourceRequestHandlerTyped<T>::makeUnique(requestType, onRequest));
    }

  private:
    const std::string resource;
    QueueController& queueController;
    std::vector<ResourceRequestHandler::Unique> handlers;

    RequestHandler getRequestHandler(const Request& request);
    RequestHandler getResourceRequestHandler(std::string requestType, std::string contentType);
};

}

#endif /* end of include guard: MESSAGING_QUEUE_RESOURCE_CONTROLLER_HPP */
