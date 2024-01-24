// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EasingTest.generated.h"

UCLASS()
class CPPTPS_API AEasingTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEasingTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compCube;

	// 현재 시간
	float currTime = 0;
	// 현재 크기(Scale) 값
	float currScale = 1.0f;
	// Lerp 의 비율
	float ratio = 0;

	// 일반적인 방법으로 크기 키우기
	void ScaleNormal();

	// Lerp(선형) 로 크기 키우기
	void ScaleLerp();

	// Easing Function 으로 크기 키우기
	void ScaleEase();
};
