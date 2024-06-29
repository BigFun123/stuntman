// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StuntmanPawn.h"
#include "StuntmanSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class STUNTMAN_API AStuntmanSportsCar : public AStuntmanPawn
{
	GENERATED_BODY()
	
public:

	AStuntmanSportsCar();
};
