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
	
	// 앞, 뒤 판별 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float dirF = 0;

	// 좌, 우 판별 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float dirR = 0;

	// 공중에 떠 있는지 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool isAir = false;
};
