// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "AnimEnemy.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UAnimEnemy : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackType attackType;
};
