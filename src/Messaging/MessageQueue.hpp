// Copyright Sergey Anisimov 2016-2017
// MIT License
//
// Gluino
// https://github.com/anisimovsergey/gluino

#ifndef MESSAGING_MESSAGE_QUEUE_HPP
#define MESSAGING_MESSAGE_QUEUE_HPP

#include "IMessageQueue.hpp"

#include <queue>
#include <list>

namespace Messaging {

/**
  The message queue.
*/
class MessageQueue : public IMessageQueue {
  TYPE_PTRS(MessageQueue)
  public:
    /**
      This method should be called for dispatching the messages.
    */
    virtual void idle() override;

    /**
      Adds a request to the queue.
    */
    virtual Core::StatusResult::Unique addRequest(Request::Shared request) override;

    /**
      Adds a event to the queue.
    */
    virtual Core::StatusResult::Unique addEvent(Event::Shared event) override;

    /**
      Creates a queue client.
    */
    virtual QueueGenericClient::Shared createGenericClient(std::string clinetId) override;

    /**
      Removes the queue client.
    */
    virtual void removeGenericClient(QueueGenericClient::Shared client) override;

    /**
    Creates a queue client.
    */
    virtual QueueResourceClient::Shared createResourceClient(std::string clinetId, std::string resource) override;

    /**
    Removes the queue client.
    */
    virtual void removeResourceClient(QueueResourceClient::Shared client) override;

    /**
      Creates a queue controller.
    */
    virtual QueueResourceController::Shared createResourceController(std::string resource) override;

    /**
      Removes the queue controller.
    */
    virtual void removeResourceController(QueueResourceController::Shared controller) override;

  private:
    std::queue<Request::Shared> requests;
    std::queue<Response::Shared> responses;
    std::queue<Event::Shared> events;

    std::list<QueueClient::Shared> clients;
    std::list<QueueResourceController::Shared> controllers;

    void processRequest(const Request& request);
    void processResponse(const Response& response);
    void processEvent(const Event& event);

    QueueClient::Shared   getClient(std::string clientId);
    RequestHandler        getRequestHandler(const Request& request);

    void sendResponseFor(
      const Request&          request,
      Core::IEntity::Unique   result
    );
};

}

#endif /* end of include guard: MESSAGING_MESSAGE_QUEUE_HPP */
