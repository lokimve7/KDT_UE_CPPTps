// Fill out your copyright notice in the Description page of Project Settings.


#include "ReferenceStudy.h"

// Sets default values
AReferenceStudy::AReferenceStudy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AReferenceStudy::BeginPlay()
{
	Super::BeginPlay();
	
	int32 number = 10;

	Test hitInfo;
	
	Test& aaa = hitInfo;
		
	ValueTypeFunc(number, hitInfo);

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay number : %d"), number);

	PointerTypeFunc(&number, &hitInfo);
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay number : %d"), number);

	ReferenceTypeFunc(number, hitInfo);
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay number : %d"), number);

}

// Called every frame
void AReferenceStudy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReferenceStudy::ValueTypeFunc(int32 n, Test t)
{
	n = 100;
	UE_LOG(LogTemp, Warning, TEXT("ValueTypeFunc n : %d"), n);

	t.age = 50;
	t.height = 200;

}

void AReferenceStudy::PointerTypeFunc(int32* p, Test* pt)
{
	*p = 200;
	UE_LOG(LogTemp, Warning, TEXT("PointerTypeFunc p : %d"), p);
	UE_LOG(LogTemp, Warning, TEXT("PointerTypeFunc p : %d"), *p);

	pt->age = 10;
	pt->height = 150;
}

void AReferenceStudy::ReferenceTypeFunc(int32& r, Test& rt)
{
	r = 300;
	UE_LOG(LogTemp, Warning, TEXT("ReferenceTypeFunc r : %d"), r);
	UE_LOG(LogTemp, Warning, TEXT("ReferenceTypeFunc r : %d"), &r);

	rt.age= 100;
	rt.height = 140;
}

