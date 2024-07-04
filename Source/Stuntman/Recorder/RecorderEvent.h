#pragma once

#include "RecorderEvent.generated.h"

USTRUCT(Blueprintable) 
struct STUNTMAN_API FRecorderEvent {
   GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Position = FVector(0,0,0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FQuat Rotation = FQuat(0,0,0,0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Velocity = FVector(0,0,0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector AngularVelocity = FVector(0,0,0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Time = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Scene = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Event = 0; // 0= nothing, 1 = explode, 2 = smoke, 3 = gas, 100 = fliprside, 101 = fliplside, 102 = flipfside, 103 = flipbside

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Type = "actor"; // 0 = car, 1 = actor, 2 = camera, 3 = explosion, 4 = smoke, 5 = gas, 6 = mark
};