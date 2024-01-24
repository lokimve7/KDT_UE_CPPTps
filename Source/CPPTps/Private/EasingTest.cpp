// Fill out your copyright notice in the Description page of Project Settings.


#include "EasingTest.h"
#include "EasingLibrary.h"

// Sets default values
AEasingTest::AEasingTest()
{
	PrimaryActorTick.bCanEverTick = true;

	compCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CUBE"));
	SetRootComponent(compCube);
}

void AEasingTest::BeginPlay()
{
	Super::BeginPlay();


	
}

void AEasingTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currTime += DeltaTime;
	if (currTime > 1)
	{
		// 크기를 서서히 2배로 한다. (1, 1, 1) -> (2, 2, 2)
		//ScaleNormal();
		//ScaleLerp();
		ScaleEase();
	}
}

void AEasingTest::ScaleNormal()
{
	currScale += GetWorld()->GetDeltaSeconds();
	if (currScale > 2) currScale = 2;

	FVector scale = FVector::OneVector * currScale;
	SetActorScale3D(scale);
}

void AEasingTest::ScaleLerp()
{
	ratio += GetWorld()->GetDeltaSeconds();
	if(ratio > 1) ratio = 1;

	/*currScale = FMath::Lerp(1.0f, 2.0f, ratio);
	FVector scale = FVector::OneVector * currScale;
	SetActorScale3D(scale);*/

	FVector scale = FMath::Lerp(FVector::OneVector, FVector::OneVector * 2, ratio);
	SetActorScale3D(scale);
}

void AEasingTest::ScaleEase()
{
	ratio += GetWorld()->GetDeltaSeconds() * 3;
	if(ratio > 1) ratio = 1;

	float s = UEasingLibrary::EaseOutBack(ratio);

	FVector scale = FMath::Lerp(FVector::OneVector, FVector::OneVector * 2, s);
	SetActorScale3D(scale);
}
