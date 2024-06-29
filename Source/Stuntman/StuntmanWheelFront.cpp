// Copyright Epic Games, Inc. All Rights Reserved.

#include "StuntmanWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UStuntmanWheelFront::UStuntmanWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}