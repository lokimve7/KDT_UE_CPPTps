// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"
#include "Enemy.h"
#include "AnimEnemy.h"
#include <Components/CapsuleComponent.h>
#include <AIController.h>


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// montage 읽어오자
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/OnlyBlueprints/EnemyMontage.EnemyMontage'"));
	if (tempMontage.Succeeded())
	{
		montage = tempMontage.Object;
	}
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

	// 나한테 설정되어 있는 Anim Class 가져오자
	USkeletalMeshComponent* mesh = myActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();
	anim = Cast<UAnimEnemy>(animInstance);

	//anim = Cast<UAnimEnemy>(myActor->GetMesh()->GetAnimInstance());

	// AI Controller 찾자
	ai = Cast<AAIController>(myActor->GetController());

	// 시야각을 cos(시야각) 으로 하자
	float radianViewAngle = FMath::DegreesToRadians(viewAngle * 0.5f);
	viewAngle = FMath::Cos(radianViewAngle);
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
	case EEnemyState::ATTACK_DELAY:
		UpdateAttackDelay();
		break;
	case EEnemyState::DAMAGE:
		UpdateDamaged(DeltaTime);
		break;
	case EEnemyState::DIE:
		UpdateDie();
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

	// 네비게이션 동작 멈춰
	ai->StopMovement();

	// 현재 상태를 갱신
	currState = s;

	// 애니메이션 상태 갱신
	anim->state = currState;
	
	// 현재 시간을 초기화
	currTime = 0;

	switch (currState)
	{
	case EEnemyState::IDLE:

		break;
	case EEnemyState::MOVE:

		break;
	case EEnemyState::ATTACK:
	{
		// 킥, 펀치 공격할지 설정
		int32 rand = FMath::RandRange(0, 1);
		anim->attackType = (EAttackType)rand;
	}
		break;
	case EEnemyState::DAMAGE:
	{
		// 1. 랜덤한 값을 뽑는다. (1, 2)
		int32 rand = FMath::RandRange(1, 2);
		// 2. Damage01, Damage02 란 문자열을 만든다.
		FString sectionName = FString::Printf(TEXT("Damage0%d"), rand);
		// 3. Montage 플레이
		myActor->PlayAnimMontage(montage, 1.0f, FName(*sectionName));
	}
		break;
	case EEnemyState::DIE:
		// 죽는 애니메이션 플레이
		myActor->PlayAnimMontage(montage, 1.0f, TEXT("Die"));
		// 충돌 처리 되지 않게 하자		
		myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// myActor->Destroy();
		break;
	default:
		break;
	}
}

void UEnemyFSM::UpdateIdle()
{
	// 만약에 플레이어를 쫓아갈 수 있다면
	if (CanTrace())
	{
		// 상태를 Move 로 바꿔라
		ChangeState(EEnemyState::MOVE);
	}	
}

void UEnemyFSM::UpdateMove()
{
	// 1. 플레이어를 향하는 방향을 구하자
	FVector dir = target->GetActorLocation() - myActor->GetActorLocation();
	

	// 2. target 위치로 움직이자 (Navigation  기능을 통해서)
	ai->MoveToLocation(target->GetActorLocation());

	//// 2. 그 방향으로 움직이자. 
	//myActor->AddMovementInput(dir.GetSafeNormal());

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
	ChangeState(EEnemyState::ATTACK_DELAY);
}

void UEnemyFSM::UpdateAttackDelay()
{	
	//// 1. 시간을 흐르게 한다.
	//currTime += GetWorld()->DeltaTimeSeconds;
	//// 2. 만약에 Attack Delay 시간이 지나면
	//if (currTime > attackDelayTime)

	if (IsWaitComplete(attackDelayTime))
	{
		// 플레어와의 거리
		float dist = FVector::Distance(target->GetActorLocation(), myActor->GetActorLocation());
		// 그 거리가 공격범위- > 진짜 공격
		if (dist < attackRange)
		{
			// 3. 공격 상태로 가라
			ChangeState(EEnemyState::ATTACK);
		}
		// 인지범위 -> 이동 
		else if (CanTrace())
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
	if (IsWaitComplete(damageDelayTime))
	{
		// IDLE 상태로 전환
		ChangeState(EEnemyState::IDLE);
	}

	//// damageDelayTime 기다렸다가
	//currTime += deltaTime;
	//if (currTime > damageDelayTime)
	//{
	//	// IDLE 상태로 전환
	//	ChangeState(EEnemyState::IDLE);
	//}
}

void UEnemyFSM::UpdateDie()
{
	if (IsWaitComplete(3))
	{
		myActor->Destroy();
	}
}

bool UEnemyFSM::IsWaitComplete(float delay)
{
	currTime += GetWorld()->DeltaTimeSeconds;
	if (currTime >= delay)
	{
		return true;
	}

	return false;
}

bool UEnemyFSM::CanTrace()
{
	// 1. 플레이어와의 거리를 구하자.
	FVector dir = target->GetActorLocation() - myActor->GetActorLocation();
	float dist = dir.Length();
	// 2. 만약에 거리가 인지범위 보다 작으면
	if (dist < traceRange)
	{
		// 나의 앞방향과 플레어를 향하는 방향을 내적
		float dot = FVector::DotProduct(dir.GetSafeNormal(), myActor->GetActorForwardVector());

		if (dot > viewAngle)
		{
			return true;
		}

		//// 내적한 값을 acos 하자
		//float radianAngle = FMath::Acos(dot);
		//// degree 값으로 변경하자
		//float degreeAngle = FMath::RadiansToDegrees(radianAngle);
		//// 만약에 플레이어가 시약각(60)에 들어왔다면 (degreeAngle <= 60 / 2) 
		//if (degreeAngle <= 180 * 0.5f)
		//{
		//	return true;
		//}
	}

	return false;
}

