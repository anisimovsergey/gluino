#include "QueueResourceController.hpp"
#include "IMessageQueue.hpp"
#include "Core/Memory.hpp"

using namespace Core;
using namespace Messaging;

QueueResourceController::QueueResourceController(const std::string& resource, IMessageQueue& messageQueue) :
  resource(resource), messageQueue(messageQueue) {
}

QueueResourceController::~QueueResourceController() {
  messageQueue.removeController(*this);
}

Core::Status
QueueResourceController::sendEvent(EventType eventType) {
  auto event = std::make_unique<Event>(eventType, resource);
  return messageQueue.addEvent(std::move(event));
}

Core::Status
QueueResourceController::sendEvent(EventType eventType, std::unique_ptr<IEntity> content) {
  auto event = std::make_unique<Event>(eventType, resource, std::move(content));
  return messageQueue.addEvent(std::move(event));
}

RequestHandler
QueueResourceController::getRequestHandler(const Request& request) const {
  if (request.getResource() != resource) {
    return nullptr;
  }

  auto requestType = request.getRequestType();
  std::string contentType;
  if (request.getContent())
    contentType = request.getContent()->getTypeId();

  for (auto& handler : handlers) {
    if (handler->getRequestType() == requestType &&
        handler->getContentType() == contentType) {
      return [&](const Request& r){ return handler->makeRequest(r); };
    }
  }
  return nullptr;
}
