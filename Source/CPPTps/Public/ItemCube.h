// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "ItemCube.generated.h"

UCLASS()
class CPPTPS_API AItemCube : public AItemObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 현재 나의 회전값 (Yaw)
	float yaw = 0;	

public:
	// 기본 동작
	virtual void NormalState() override;

	// 총에 맞았을때 해야하는 행동
	virtual void OnHit() override;

};
