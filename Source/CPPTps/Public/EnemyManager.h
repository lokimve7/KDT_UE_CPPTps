// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class CPPTPS_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	TArray<AActor*> allSpawnPos;

	// Enemy Class 
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AEnemy> enemyFactory;

	float currTime = 0;
	float createTime = 2;

	// 어떤 SpawnPos 에서 나와야 하는지
	int32 spawnIdx = 0;
};
