// Copyright Sergey Anisimov 2016-2017
// MIT License
//
// Gluino
// https://github.com/anisimovsergey/gluino

#ifndef MESSAGING_QUEUE_RESOURCE_CONTROLLER_HPP
#define MESSAGING_QUEUE_RESOURCE_CONTROLLER_HPP

#include "QueueController.hpp"
#include "Core/Logger.hpp"

#include <set>

namespace Messaging {

class IMessageQueue;

template<typename T>
class QueueResourceController {
  TYPE_PTRS(QueueResourceController<T>)
  typedef typename T::Unique TUnique;

  public:

    explicit QueueResourceController(QueueController::Shared queueController) :
      queueController(queueController), typeId(T::TypeId) {
      queueController->setCanProcessRequest(
        std::bind(&QueueResourceController<T>::canProcessReqiest, this, std::placeholders::_1));
      queueController->setProcessRequest(
        std::bind(&QueueResourceController<T>::processRequest, this, std::placeholders::_1));
    }

    void setOnGetRequestHandler(std::function<Core::ActionResult::Unique()> onGetRequestHandler) {
      this->onGetRequestHandler = onGetRequestHandler;
    }

    void setOnCreateRequestHandler(std::function<Core::StatusResult::Unique(const T&)> onCreateRequestHandler) {
      this->onCreateRequestHandler = onCreateRequestHandler;
    }

    void setOnUpdateRequestHandler(std::function<Core::StatusResult::Unique(const T&)> onUpdateRequestHandler) {
      this->onUpdateRequestHandler = onUpdateRequestHandler;
    }

    void setOnDeleteRequestHandler(std::function<Core::StatusResult::Unique()> onDeleteRequestHandler) {
      this->onDeleteRequestHandler = onDeleteRequestHandler;
    }

    void sendGetNotification(Core::ActionResult::Shared result) {
      for(auto sender: senders) {
        queueController->sendNotification(sender, Action::Get, typeId, result);
      }
      senders.clear();
    }

    void sendCreateNotification(TUnique object) {
      queueController->broadcastNotification(Action::Create, typeId, std::move(object));
    }

    void sendUpdateNotification(TUnique object) {
      queueController->broadcastNotification(Action::Update, typeId, std::move(object));
    }

    void sendDeleteNotification() {
      auto result = StatusResult::makeUnique(StatusCode::NoContent, "The resource was deleted");
      queueController->broadcastNotification(Action::Delete, typeId, std::move(result));
    }

  private:
    QueueController::Shared queueController;
    std::string typeId;
    std::function<Core::ActionResult::Unique()> onGetRequestHandler;
    std::function<Core::StatusResult::Unique(const T&)> onCreateRequestHandler;
    std::function<Core::StatusResult::Unique(const T&)> onUpdateRequestHandler;
    std::function<Core::StatusResult::Unique()> onDeleteRequestHandler;

    bool canProcessReqiest(const Request& request) {
      return (request.getResource() == typeId);
    }

    ActionResult::Unique processRequest(const Request& request) {
      if (request.getAction() == Action::Get) {
        if (onGetRequestHandler) {
          return onGetRequestHandler();
        }
        return StatusResult::NotImplemented();
      }
      if (request.getAction() == Action::Create) {
        if (onCreateRequestHandler) {
          auto object = T::cast(request.getContent());
          if (object)
            return onCreateRequestHandler(*object);
          return StatusResult::makeUnique(StatusCode::BadRequest, "Expeceted content of '" + std::string(T::TypeId) + "' type.");
        }
        return StatusResult::NotImplemented();
      }
      if (request.getAction() == Action::Update) {
        if (onUpdateRequestHandler) {
          auto object = T::cast(request.getContent());
          if (object)
            return onUpdateRequestHandler(*object);
          return StatusResult::makeUnique(StatusCode::BadRequest, "Expeceted content of '" + std::string(T::TypeId) + "' type.");
        }
        return StatusResult::NotImplemented();
      }
      if (request.getAction() == Action::Delete) {
        if (onDeleteRequestHandler)
          return onDeleteRequestHandler();
        return StatusResult::NotImplemented();
      }
      return StatusResult::makeUnique(StatusCode::NotImplemented, "Request action type is not supported.");
    }
};

}

#endif /* end of include guard: MESSAGING_QUEUE_RESOURCE_CONTROLLER_HPP */
