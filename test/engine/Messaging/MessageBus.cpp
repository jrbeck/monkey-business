#include "../../source/engine/Messaging/MessageBus.h"

// this is just a placeholder ...
// these tests are in no way complete

// {
//   // testing the message bus * * * * * * * * * * * * * * * * * * * * * * * *
//   printf("Messaging Stuff:\n");
//
//   EngineXYZ::Messaging::Message message;
//   EngineXYZ::Messaging::Message* receivedMessage;
//
//   int recipientId = mMessageBus->newRecipient();
//   receivedMessage = mMessageBus->readNextMessage(recipientId);
//
//   if (receivedMessage == nullptr) {
//     printf("No message ... passed\n");
//   }
//   else {
//     printf("No message ... failed\n");
//   }
//
//   message.mSenderId = recipientId;
//   message.mType = 1;
//   message.mIntValue = 2;
//   mMessageBus->sendMessage(recipientId, message);
//
//   receivedMessage = mMessageBus->readNextMessage(recipientId);
//
//   printf("id: %d\n", recipientId);
//   if (receivedMessage == nullptr) {
//     printf("One message ... failed\n");
//   }
//   else {
//     receivedMessage->print();
//   }

  // // mark as read test
  // receivedMessage = mMessageBus->readNextMessage(recipientId);
  // if (receivedMessage == nullptr) {
  //   printf("After reading message ... passed\n");
  // }
  // else {
  //   printf("After reading message ... failed\n");
  // }

// }
