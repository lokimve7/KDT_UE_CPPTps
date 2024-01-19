// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ���ʹ� Ŭ���� ��������
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
	// World �� �ִ� Actor ���� ã0�ƿ���
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), tempActor);

	// SpawnPos �� �����
	for (int32 i = 0; i < tempActor.Num(); i++)
	{
		/*if (tempActor[i]->GetName().Contains(TEXT("Spawn")) == false) continue;
		allSpawnPos.Add(tempActor[i]);*/

		// ���࿡ �̸��� Spawn �� ���ԵǾ� �ִٸ�
		if (tempActor[i]->GetActorLabel().Contains(TEXT("Spawn")))
		{
			// allSpawnPos �� �߰�
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
		// ���ʹ� ���� (��ġ : SpawnPos �߿� �ϳ�)
		//int32 rand = FMath::RandRange(0, allSpawnPos.Num() - 1);
		GetWorld()->SpawnActor<AEnemy>(
			enemyFactory,
			allSpawnPos[spawnIdx]->GetActorLocation(),
			FRotator(0)
		);

		// spawnIdx �� ������
		spawnIdx ++;
		if (spawnIdx == allSpawnPos.Num())
		{
			spawnIdx = 0;
		}

		//spawnIdx = (spawnIdx + 1) % allSpawnPos.Num();
		//spawnIdx++;
		//spawnIdx %= allSpawnPos.Num();

		// ���ð� �ʱ�ȭ
		currTime = 0;
	}
}

