// BFG

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SM_PropMaster.generated.h"

	UENUM(BlueprintType)
	enum class PropType : uint8
	{	
		Barrel = 0 UMETA(DisplayName = "Barrel"),
		Box = 1 UMETA(DisplayName = "Box"),		
		Car = 2 UMETA(DisplayName = "Car"),	
		Crate = 3 UMETA(DisplayName = "Crate"),
		Prop = 4 UMETA(DisplayName = "Prop"),
		StaticMesh = 5 UMETA(DisplayName = "StaticMesh"),
		Actor = 6 UMETA(DisplayName = "Actor"),
		Marker = 7 UMETA(DisplayName = "Marker"),
	};

/**
 * 
 */
UCLASS()
class STUNTMAN_API USM_PropMaster : public UUserWidget
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void SpawnProp(PropType PropType);
	
};
