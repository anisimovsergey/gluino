// Copyright Sergey Anisimov 2016-2017
// MIT License
//
// Gluino
// https://github.com/anisimovsergey/gluino

#ifndef MESSAGING_RESPONSE_HPP
#define MESSAGING_RESPONSE_HPP

#include "RequestType.hpp"
#include "Core/IEntity.hpp"

#include <memory>

namespace Messaging {

/**
  The response message.
*/
class Response : public Core::IEntity {
  TYPE_INFO(Response, IEntity, "response")
  public:
    /**
      Constructs a response.
    */
    Response(
      const std::string& id,
      const std::string& receiver,
      const RequestType& requestType,
      const std::string& resource,
      std::unique_ptr<IEntity> content
    );

    /**
      The request id.
    */
    std::string getId() const { return id; };

    /**
      The response receiver.
    */
    std::string getReceiver() const { return receiver; };

    /**
      Set the response receiver.
    */
    void setReceiver(std::string value) { receiver = value; };

    /**
      The request type (create, get, update, delete etc.).
    */
    RequestType getRequestType() const { return requestType; }

    /**
      The resource identifier.
    */
    std::string getResource() const { return resource; }

    /**
      The response content.
    */
    const IEntity& getContent() const { return *content; }

  private:
    std::string     id;
    std::string     receiver;
    RequestType     requestType;
    std::string     resource;
    std::unique_ptr<IEntity> content;
};

}

#endif /* end of include guard: MESSAGING_RESPONSE_HPP */
