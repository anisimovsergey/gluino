// Copyright Sergey Anisimov 2016-2017
// MIT License
//
// Gluino
// https://github.com/anisimovsergey/gluino

#ifndef MESSAGING_RESPONSE_HPP
#define MESSAGING_RESPONSE_HPP

#include "Core/IEntity.hpp"

namespace Messaging {

/**
  The response message.
*/
class Response : public Core::IEntity {
  TYPE_INFO(Response, IEntity, "response")
  public:
    Response(
      std::string       requestType,
      std::string       sender,
      std::string       receiver,
      std::string       resource,
      IEntity::Shared   content
    );

    /**
      The request type (create, get, update, delete etc.).
    */
    std::string getRequestType() const { return requestType; }

    /**
      The response sender.
    */
    std::string getSender() const { return sender; };

    /**
      The response receiver.
    */
    std::string getReceiver() const { return receiver; };

    /**
      The resource identifier.
    */
    std::string getResource() const { return resource; }

    /**
      The response content.
    */
    const IEntity& getContent() const { return *content; }

  private:
    std::string     requestType;
    std::string     sender;
    std::string     receiver;
    std::string     resource;
    IEntity::Shared content;
};

}

#endif /* end of include guard: MESSAGING_RESPONSE_HPP */
