// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimPlayer.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UAnimPlayer : public UAnimInstance
{
	GENERATED_BODY()

public:	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	// 플레이동 속력
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float moveSpeed = 0;

};
