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
    float Frame = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Scene = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int Event = 0; // 0 = nothing, 1 = explode, 2 = extinguish, 3 = smoke, 4 = gas, 100 = fliprside, 101 = fliplside, 102 = flipfside, 103 = flipbside

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Type = "actor"; // class name
};