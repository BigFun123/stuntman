// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SM_Timeline.generated.h"

/**
 * 
 */
UCLASS()
class STUNTMAN_API USM_Timeline : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	bool Recording = false;	

	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void StartRecording();

	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void StopRecording();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	bool Playing = false;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	//float Time = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
	int Frame = 0;

	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void GotoFrame(float NewFrame);

	UFUNCTION(BlueprintCallable, Category = "Timeline")
	void Detonate(bool bDetonate);

protected:
	UFUNCTION(BlueprintCallable, Category = "Timeline")
	float GetFrame();	

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
