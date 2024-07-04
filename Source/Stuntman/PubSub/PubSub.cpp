#include "PubSub.h"
PubSub::PubSub()
{
	//ctor
}

PubSub::~PubSub()
{
	//dtor
}

void PubSub::addSubscriber(ISubscriber* pSubscriber) {
	subscribers.push_back(pSubscriber);
}

void PubSub::removeSubscriber(ISubscriber* pSubscriber) {
	for (int i = 0; i < subscribers.size(); i++) {
		if (subscribers[i] == pSubscriber) {
			subscribers.erase(subscribers.begin() + i);
			break;
		}
	}
}

PubSub& PubSub::GetInstance() {	
	static PubSub instance;
	return instance;
}

void PubSub::Subscribe(ISubscriber* pSubscriber) {
	PubSub::GetInstance().addSubscriber(pSubscriber);
}

void PubSub::Unsubscribe(ISubscriber* pSubscriber) {
	PubSub::GetInstance().removeSubscriber(pSubscriber);
}

void PubSub::Send(PubSubMessage& payload) {
	auto GetInstancesubscribers = PubSub::GetInstance().subscribers;	
	for (int i = 0; i < GetInstancesubscribers.size(); i++) {
		if (GetInstancesubscribers[i] != nullptr) {
			GetInstancesubscribers[i]->onMessage(payload);
		}	
	}	
}