// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSphere.generated.h"

UCLASS()
class CPPTPS_API AItemSphere : public AActor
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
	// ����
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* compMesh;

	// ���� ũ�� ��
	float scale = 1;
	// Ŀ���� / �۾����� �Ѵ� �ӷ�
	float speed = 5;

	// ���� Ŀ���� ���� / �۾����� ����
	bool isGrow = true;
	int32 scaleDir = 1;

	// NormalState �� �����ϰ� �Ұ��� ���� ��������
	bool isNormal = true;

public:
	// �⺻ ����
	void NormalState();

	// �ѿ� �¾����� �ؾ��ϴ� �ൿ
	void OnHit();
};
