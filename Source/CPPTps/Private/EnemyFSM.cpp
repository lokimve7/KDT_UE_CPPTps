// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"
#include "Enemy.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어를 찾자
	AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass());
	target = Cast<ATpsPlayer>(findActor);
	
	// 나의 액터를 찾자
	myActor = Cast<AEnemy>(GetOwner());

	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 만약에 Target 이 없다면 함수를 나가자
	if (target == nullptr) return;

	switch (currState)
	{
	case EEnemyState::IDLE:
		UpdateIdle();
		break;
	case EEnemyState::MOVE:
		UpdateMove();
		break;
	case EEnemyState::ATTACK:
		UpdateAttack();
		break;
	case EEnemyState::DAMAGE:
		UpdateDamaged(DeltaTime);
		break;
	default:
		break;
	}
}

void UEnemyFSM::ChangeState(EEnemyState s)
{
	// 바뀌는 상태를 출력하자
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)s));
	}	

	// 현재 상태를 갱신
	currState = s;
	
	// 현재 시간을 초기화
	currTime = 0;

	switch (currState)
	{
	case EEnemyState::IDLE:

		break;
	case EEnemyState::MOVE:

		break;
	case EEnemyState::ATTACK:
		// 바로 공격 가능하게 현재시간을 attackDelayTime 으로 설정
		currTime = attackDelayTime;
		break;
	case EEnemyState::DIE:
		myActor->Destroy();
		break;
	default:
		break;
	}
}

void UEnemyFSM::UpdateIdle()
{
	// 1. 플레이어와의 거리를 구하자.
	FVector dir = target->GetActorLocation() - myActor->GetActorLocation();
	float dist = dir.Length();
	// 2. 만약에 거리가 인지범위 보다 작으면 (플레이어를 쫓아갈 수 있는 상태)
	if (dist < traceRange)
	{
		// 3. 현재 상태를 MOVE 로 바꾸자
		ChangeState(EEnemyState::MOVE);
	}
}

void UEnemyFSM::UpdateMove()
{
	// 1. 플레이어를 향하는 방향을 구하자
	FVector dir = target->GetActorLocation() - myActor->GetActorLocation();
	
	// 2. 그 방향으로 움직이자. 
	myActor->AddMovementInput(dir.GetSafeNormal());

	// 3. 플레이어와의 거리가 공격 범위보다 작으면
	float dist = dir.Length();
	if (dist < attackRange)
	{
		// 4. 현재 상태를 ATTACK 로 바꾸자
		ChangeState(EEnemyState::ATTACK);	
	}	
}

void UEnemyFSM::UpdateAttack()
{
	// 1. 시간을 흐르게 한다.
	currTime += GetWorld()->DeltaTimeSeconds;
	// 2. 만약에 Attack Delay 시간이 지나면
	if (currTime > attackDelayTime)
	{
		// 플레어와의 거리
		float dist = FVector::Distance(target->GetActorLocation(), myActor->GetActorLocation());
		// 그 거리가 공격범위- > 진짜 공격
		if (dist < attackRange)
		{
			// 3. 진짜 공격!!
			UE_LOG(LogTemp, Warning, TEXT("Real ATTACK"));
		}
		// 인지범위 -> 이동 
		else if (dist < traceRange)
		{
			ChangeState(EEnemyState::MOVE);
		}
		// 그 외는 -> 대기
		else
		{
			ChangeState(EEnemyState::IDLE);
		}
		
		// 4. 현재시간 초기화
		currTime = 0;
	}
}

void UEnemyFSM::UpdateDamaged(float deltaTime)
{
	// damageDelayTime 기다렸다가
	currTime += deltaTime;
	if (currTime > damageDelayTime)
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}
}

