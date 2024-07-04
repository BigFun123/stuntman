#pragma once

#include "PubSubMessage.h"

/**
 * Interface for subscribers to implement
 * Implement this interface to receive messages from the PUBSUB message bus  
 */
class ISubscriber {
public:
	virtual void onMessage(PubSubMessage& payload) = 0;	
};