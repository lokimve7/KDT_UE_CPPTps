// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemCube.generated.h"

UCLASS()
class CPPTPS_API AItemCube : public AActor
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
	// 외형
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

	// 현재 나의 회전값 (Yaw)
	float yaw = 0;
	// 회전 한는 속력
	float speed = 50;

public:
	// 기본 동작
	void NormalState();

	// 총에 맞았을때 해야하는 행동
	void OnHit();

};
