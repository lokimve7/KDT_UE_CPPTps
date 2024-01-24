// Fill out your copyright notice in the Description page of Project Settings.


#include "EasingTest.h"

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

	float s = EaseOutBack(ratio);

	FVector scale = FMath::Lerp(FVector::OneVector, FVector::OneVector * 2, s);
	SetActorScale3D(scale);
}

float AEasingTest::EaseOutBounce(float x)
{
	const float n1 = 7.5625;
	const float d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

float AEasingTest::EaseOutBack(float x)
{
	const float c1 = 1.70158;
	const float c3 = c1 + 1;

	return 1 + c3 * FMath::Pow(x - 1, 3) + c1 * FMath::Pow(x - 1, 2);
}

