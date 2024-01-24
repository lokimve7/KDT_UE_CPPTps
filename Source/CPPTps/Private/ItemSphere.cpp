// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSphere.h"

// Sets default values
AItemSphere::AItemSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemSphere::BeginPlay()
{
	Super::BeginPlay();

	speed = 5;
}

// Called every frame
void AItemSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isNormal == true)
	{
		NormalState();
	}
}

void AItemSphere::NormalState()
{
	Super::NormalState();

	// scaleDir 방향으로 크기를 변경시켜줘
	scale += speed * GetWorld()->GetDeltaSeconds() * scaleDir;

	SetActorScale3D(FVector(scale));
	// 만약에 scale 값이 2 보다 커지거나 
	// sclae 값이 1보다 작아지면
	if (scale > 2 || scale < 1)
	{
		// scale 값이 최소 : 1, 최대 : 2
		scale = FMath::Clamp(scale, 1, 2);

		// scaleDir 의 방향을 바꿔라 ( * -1)
		scaleDir *= -1;
	}


	//// 만약에 커지는 상태라면
	//if (isGrow == true)
	//{
	//	// scale 이 2 가 될때까지 커지자
	//	scale += speed * GetWorld()->GetDeltaSeconds();
	//	SetActorScale3D(FVector(scale));

	//	// 만약에 scale 값이 2 보다 커지면
	//	if (scale > 2)
	//	{
	//		// 작아지는 상태로 바꿔라
	//		isGrow = false;
	//	}
	//}

	//// 그렇지 않고 작아지는 상태라면
	//else
	//{
	//	// scale 이 1 이 될때까지 작아지자
	//	scale -= speed * GetWorld()->GetDeltaSeconds();
	//	SetActorScale3D(FVector(scale));

	//	// 만약에 scale 값이 1 보다 작아지면
	//	if (scale < 1)
	//	{
	//		// 커지는 상태로 바꿔라
	//		isGrow = true;
	//	}
	//}
}

void AItemSphere::OnHit()
{
	Super::OnHit();

	// isNormal 반대로 동작하게 하자
	isNormal = !isNormal;
}

