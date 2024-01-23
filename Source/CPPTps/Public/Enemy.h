// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class CPPTPS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
	class UEnemyFSM* fsm;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* comHpBar;

	UPROPERTY(EditAnywhere)
	int32 maxHP = 3;
	int32 currHP = 0;

public:
	void DamageProcess(int32 damage);
	void AttackTarget();
};
