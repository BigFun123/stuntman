#pragma once

/**
 * Interface for subscribers to implement
 * Implement this interface to receive messages from the PUBSUB message bus  
 */
class ISubscriber {
public:
	virtual void onMessage(const FName& message) = 0;	
};