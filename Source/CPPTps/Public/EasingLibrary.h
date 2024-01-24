// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EasingLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UEasingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static float EaseOutBounce(float x);

	UFUNCTION(BlueprintCallable)
	static float EaseOutBack(float x);
	
};
