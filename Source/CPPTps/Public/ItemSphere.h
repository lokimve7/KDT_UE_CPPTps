// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemObject.h"
#include "ItemSphere.generated.h"

UCLASS()
class CPPTPS_API AItemSphere : public AItemObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:	

	// 현재 크기 값
	float scale = 1;
	
	// 내가 커지는 상태 / 작아지는 상태
	bool isGrow = true;
	int32 scaleDir = 1;

	// NormalState 를 실행하게 할건지 하지 않을건지
	bool isNormal = true;

public:
	// 기본 동작
	virtual void NormalState() override;

	// 총에 맞았을때 해야하는 행동
	virtual void OnHit() override;
};
