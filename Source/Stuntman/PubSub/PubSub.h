#pragma once
#include <vector>
#include "Subscriber.h"

class PubSub
{
	private:
	// singleton instance
	//static PubSub& instance;
	// list of subscribers
	std::vector<ISubscriber*> subscribers;	
	public:
		PubSub();
		~PubSub();

		// add a subscriber
		void addSubscriber(ISubscriber* subscriber);
		void removeSubscriber(ISubscriber* pSubscriber);
		
		// remove a subscriber
		/*void removeSubscriber(ISubscriber* subscriber);
		// notify all subscribers
		void notifySubscribers();
		// notify a specific subscriber
		void notifySubscriber(ISubscriber* subscriber);
		// virtual destructor
		virtual ~PubSub();*/
		static PubSub& GetInstance();
		static void Subscribe(ISubscriber* subscriber);
		static void Unsubscribe(ISubscriber* subscriber);
		static void Send(const FName& message);
};

