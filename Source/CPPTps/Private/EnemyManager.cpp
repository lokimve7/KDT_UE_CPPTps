// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 에너미 클래스 가져오자
	ConstructorHelpers::FClassFinder<AEnemy> temp(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Enemy.BP_Enemy_C'"));
	if (temp.Succeeded())
	{
		enemyFactory = temp.Class;
	}
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> tempActor;
	// World 에 있는 Actor 들을 찾0아오자
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), tempActor);

	// SpawnPos 만 골라내자
	for (int32 i = 0; i < tempActor.Num(); i++)
	{
		/*if (tempActor[i]->GetName().Contains(TEXT("Spawn")) == false) continue;
		allSpawnPos.Add(tempActor[i]);*/

		// 만약에 이름에 Spawn 이 포함되어 있다면
		if (tempActor[i]->GetActorLabel().Contains(TEXT("Spawn")))
		{
			// allSpawnPos 에 추가
			allSpawnPos.Add(tempActor[i]);
		}
	}
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currTime += DeltaTime;

	if (currTime > createTime)
	{
		// 에너미 생성 (위치 : SpawnPos 중에 하나)
		//int32 rand = FMath::RandRange(0, allSpawnPos.Num() - 1);
		GetWorld()->SpawnActor<AEnemy>(
			enemyFactory,
			allSpawnPos[spawnIdx]->GetActorLocation(),
			FRotator(0)
		);

		// spawnIdx 를 다음로
		spawnIdx ++;
		if (spawnIdx == allSpawnPos.Num())
		{
			spawnIdx = 0;
		}

		//spawnIdx = (spawnIdx + 1) % allSpawnPos.Num();
		//spawnIdx++;
		//spawnIdx %= allSpawnPos.Num();

		// 현시간 초기화
		currTime = 0;
	}
}

