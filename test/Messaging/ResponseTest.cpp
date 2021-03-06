#include "Utils/Testing.hpp"

#include "Core/Memory.hpp"
#include "Core/Status.hpp"
#include "Messaging/Response.hpp"

using namespace Core;
using namespace Messaging;

namespace {

  class Content : public Core::IEntity {
    TYPE_INFO(Content, Core::IEntity, "content")
  };

}

TEST_CASE("Response can be constructed", "[Response]") {
  auto content = std::make_unique<Content>();
  auto contentPtr = content.get();

  auto response = std::make_unique<Response>("id", "receiver", RequestType::Read, "resource", std::move(content));

  SECTION("id is correct") {
    REQUIRE(response->getId() == "id");
  }

  SECTION("type is correct") {
    REQUIRE(response->getTypeId() == "response");
  }

  SECTION("receiver retained") {
	  REQUIRE(response->getReceiver() == "receiver");
  }

  SECTION("request type retained") {
    REQUIRE(response->getRequestType() == RequestType::Read);
  }

  SECTION("resource retained") {
    REQUIRE(response->getResource() == "resource");
  }

  SECTION("content retained") {
    REQUIRE(&response->getContent() == contentPtr);
  }
}

TEST_CASE("Receiver can be changed", "[Response]") {
	auto response = std::make_unique<Response>("id", "receiver", RequestType::Read, "resource", std::make_unique<Content>());

	response->setReceiver("new_receiver");

	REQUIRE(response->getReceiver() == "new_receiver");
}
