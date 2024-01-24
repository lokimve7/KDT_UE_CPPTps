// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemCube.h"
#include "Inventory.h"

// Sets default values
AItemCube::AItemCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	itemType = EItemType::ITEM_CUBE;
}

// Called when the game starts or when spawned
void AItemCube::BeginPlay()
{
	Super::BeginPlay();
	
	speed = 50;
}

// Called every frame
void AItemCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	NormalState();
}

void AItemCube::NormalState()
{
	Super::NormalState();

	// yaw ���� ����
	yaw += speed * GetWorld()->GetDeltaSeconds();
	
	FRotator rot = FRotator(0, yaw, 0);
	// rot ������ ȸ���� ����
	SetActorRotation(rot);
}

void AItemCube::OnHit()
{
	Super::OnHit();

	speed += 5;
}

