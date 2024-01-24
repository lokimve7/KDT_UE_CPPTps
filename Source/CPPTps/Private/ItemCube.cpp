// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCube.h"

// Sets default values
AItemCube::AItemCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���� ����
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	SetRootComponent(compMesh);

}

// Called when the game starts or when spawned
void AItemCube::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NormalState();
}

void AItemCube::NormalState()
{
	// yaw ���� ����
	yaw += speed * GetWorld()->GetDeltaSeconds();
	
	FRotator rot = FRotator(0, yaw, 0);
	// rot ������ ȸ���� ����
	SetActorRotation(rot);
}

void AItemCube::OnHit()
{
	speed += 5;
}

