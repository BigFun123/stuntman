#pragma once
#include <vector>
#include "Subscriber.h"
#include "PubSubMessage.h"

/**
 * PUBSUB message bus 
 * Singleton class that allows objects to subscribe to messages
 * To send a message, call PubSub::Send(FName("message"));
 * To subscribe to a message, call PubSub::Subscribe(this);
 * To unsubscribe from a message, call PubSub::Unsubscribe(this); You must implement this in your destructor
 * To handle a message, add the ISubscriber interface to your class, and implement the onMessage method
 */
class PubSub
{
	private:	
	// list of subscribers
	std::vector<ISubscriber*> subscribers;	
	public:
		PubSub();
		~PubSub();
	protected:
		// add a subscriber
		void addSubscriber(ISubscriber* subscriber);
		void removeSubscriber(ISubscriber* pSubscriber);
		static PubSub& GetInstance();
		
	public:		
		// send a message to all subscribers
		static void Subscribe(ISubscriber* subscriber);
		static void Unsubscribe(ISubscriber* subscriber);
		static void Send(PubSubMessage& payload);
};

