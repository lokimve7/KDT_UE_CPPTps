// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemObject.generated.h"

UCLASS()
class CPPTPS_API AItemObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemObject();

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

	// 속력
	float speed = 0;

public:
	// 기본 동작
	virtual void NormalState();

	// 총에 맞았을때 해야하는 행동
	virtual void OnHit();// = 0; // 순수 가상함수
};
