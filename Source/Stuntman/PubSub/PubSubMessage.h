#pragma once

struct PubSubMessage {
    public:
    FString message;
    int ipayload;
    float fpayload;
    FString spayload;
    UObject* opayload;
} ;