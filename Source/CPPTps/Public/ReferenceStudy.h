// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReferenceStudy.generated.h"

class Test
{
public:
	float height;	//4byte
	int64 age;		//8byte
};


UCLASS()
class CPPTPS_API AReferenceStudy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReferenceStudy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ValueTypeFunc(int32 n, Test t);
	void PointerTypeFunc(int32* p, Test* pt);
	void ReferenceTypeFunc(int32& r, Test& rt);

	
};
