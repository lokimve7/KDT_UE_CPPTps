// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/SphereComponent.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체
	compSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	SetRootComponent(compSphere);
	compSphere->SetSphereRadius(13);
	compSphere->SetCollisionProfileName(TEXT("BlockAll"));

	// 모양
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	compMesh->SetupAttachment(RootComponent);
	compMesh->SetRelativeScale3D(FVector(0.26f));
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial> tempMat(TEXT("/Script/Engine.Material'/Engine/EditorMaterials/Cascade/CASC_RenderNormal_MAT.CASC_RenderNormal_MAT'"));
	if (tempMat.Succeeded())
	{
		compMesh->SetMaterial(0, tempMat.Object);
	}

	// 발사체 움직임 관리하는 컴포넌트
	compProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));
	// 움직일 콤포넌트 설정
	compProjectile->SetUpdatedComponent(compSphere);
	// 초기 속력, 최대 속력
	compProjectile->InitialSpeed = 1500;
	compProjectile->MaxSpeed = 1500;
	// 튕기게 할것인지
	compProjectile->bShouldBounce = true;
	// 얼마나 잘 튕기게 할것인지
	compProjectile->Bounciness = 0.6f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	 
	SetLifeSpan(2);

	//UKismetSystemLibrary::K2_SetTimer(this, TEXT("AutoDestroy"), 2, false);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//// 1. 시간을 흐르게 하자
	//currTime += DeltaTime;
	//// 2. 2초가 지나면
	//if (currTime > 2)
	//{
	//	// 3. 나를 파괴하자
	//	Destroy();
	//}
}

void ABullet::AutoDestroy()
{
	Destroy();
}

