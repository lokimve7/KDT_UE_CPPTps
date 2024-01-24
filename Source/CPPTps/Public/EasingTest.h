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

	// ���� �ð�
	float currTime = 0;
	// ���� ũ��(Scale) ��
	float currScale = 1.0f;
	// Lerp �� ����
	float ratio = 0;

	// �Ϲ����� ������� ũ�� Ű���
	void ScaleNormal();

	// Lerp(����) �� ũ�� Ű���
	void ScaleLerp();

	// Easing Function ���� ũ�� Ű���
	void ScaleEase();
};
