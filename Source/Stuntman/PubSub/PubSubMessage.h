#pragma once

struct PubSubMessage {
    public:
    FString message;
    bool bpayload;
    int ipayload;
    int ipayload2;
    float fpayload;
    FString spayload;
    void* opayload;
} ;