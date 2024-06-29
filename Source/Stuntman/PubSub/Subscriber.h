#pragma once
class ISubscriber {
public:
	virtual void onMessage(const FName& message) = 0;	
};