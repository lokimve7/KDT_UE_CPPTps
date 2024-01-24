// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"

// Sets default values
AItemObject::AItemObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 외형 설정
	compMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	SetRootComponent(compMesh);

}

// Called when the game starts or when spawned
void AItemObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemObject::NormalState()
{
	
}

void AItemObject::OnHit()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHit"));
}

