// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"
#include "Enemy.h"
#include "AnimEnemy.h"
#include <Components/CapsuleComponent.h>
#include <AIController.h>
#include "Navigation/PathFollowingComponent.h"
#include <Kismet/KismetMathLibrary.h>


// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// montage �о����
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

	// �÷��̾ ã��
	AActor* findActor = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass());
	target = Cast<ATpsPlayer>(findActor);
	
	// ���� ���͸� ã��
	myActor = Cast<AEnemy>(GetOwner());

	// ������ �����Ǿ� �ִ� Anim Class ��������
	USkeletalMeshComponent* mesh = myActor->GetMesh();
	UAnimInstance* animInstance = mesh->GetAnimInstance();
	anim = Cast<UAnimEnemy>(animInstance);

	//anim = Cast<UAnimEnemy>(myActor->GetMesh()->GetAnimInstance());

	// AI Controller ã��
	ai = Cast<AAIController>(myActor->GetController());

	// �þ߰��� cos(�þ߰�) ���� ����
	float radianViewAngle = FMath::DegreesToRadians(viewAngle * 0.5f);
	viewAngle = FMath::Cos(radianViewAngle);

	// ���� ó�� ��ġ�� ��Ƶ���
	originPos = myActor->GetActorLocation();
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���࿡ Target �� ���ٸ� �Լ��� ������
	if (target == nullptr) return;

	switch (currState)
	{
	case EEnemyState::IDLE:
		UpdateIdle();
		break;
	case EEnemyState::MOVE:
		UpdateMove();
		break;
	case EEnemyState::PATROL:
		UpdatePatrol();
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
	case EEnemyState::RETURN:
		UpdateReturn();
		break;
	default:
		break;
	}
}

void UEnemyFSM::ChangeState(EEnemyState s)
{
	// �ٲ�� ���¸� �������
	UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EEnemyState"), true);
	if (enumPtr != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s -------> %s"),
			*enumPtr->GetNameStringByIndex((int32)currState),
			*enumPtr->GetNameStringByIndex((int32)s));
	}	

	// �׺���̼� ���� ����
	ai->StopMovement();

	// ���� ���¸� ����
	currState = s;

	// �ִϸ��̼� ���� ����
	anim->state = currState;
	
	// ���� �ð��� �ʱ�ȭ
	currTime = 0;

	switch (currState)
	{
	case EEnemyState::IDLE:
		break;
	case EEnemyState::MOVE:
		break;
	case EEnemyState::PATROL:
	{
		// originPos �������� �ݰ� 500 cm ���� ������ ��ġ�� �̾Ƽ� �� ��ġ�� �̵��ϰ��Ѵ�.
		// 1. ������ ������ ����
		int32 randAngle = FMath::RandRange(0, 359);
		FRotator rot = FRotator(0, randAngle, 0);
		FVector randDir = UKismetMathLibrary::GetForwardVector(rot);
		// 2. �� �������� ������ �Ÿ��� ����
		float randDist = FMath::RandRange(100.0f, 500.0f);		
		// 3. 1, 2 �� ���� �̿��ؼ� ������ ��ġ�� ����
		patrolPos = originPos + randDir * randDist;
		// �� ��ġ�� �̵�!
		ai->MoveToLocation(patrolPos);
	}
		break;
	case EEnemyState::ATTACK:
	{
		// ű, ��ġ �������� ����
		int32 rand = FMath::RandRange(0, 1);
		anim->attackType = (EAttackType)rand;
	}
		break;
	case EEnemyState::DAMAGE:
	{
		// 1. ������ ���� �̴´�. (1, 2)
		int32 rand = FMath::RandRange(1, 2);
		// 2. Damage01, Damage02 �� ���ڿ��� �����.
		FString sectionName = FString::Printf(TEXT("Damage0%d"), rand);
		// 3. Montage �÷���
		myActor->PlayAnimMontage(montage, 1.0f, FName(*sectionName));
	}
		break;
	case EEnemyState::DIE:
		// �״� �ִϸ��̼� �÷���
		myActor->PlayAnimMontage(montage, 1.0f, TEXT("Die"));
		// �浹 ó�� ���� �ʰ� ����		
		myActor->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// myActor->Destroy();
		break;
	case EEnemyState::RETURN:
		ai->MoveToLocation(originPos);
		break;
	default:
		break;
	}
}

void UEnemyFSM::UpdateIdle()
{
	// ���࿡ �÷��̾ �Ѿư� �� �ִٸ�
	if (CanTrace())
	{
		// ���¸� Move �� �ٲ��
		ChangeState(EEnemyState::MOVE);
	}	
	// �׷��� �ʰ� idleDelayTime �� �����ٸ�
	else if (IsWaitComplete(idleDelayTime))
	{
		// ���¸� PATROL �� �ٲ��
		ChangeState(EEnemyState::PATROL);
	}
}

void UEnemyFSM::UpdateMove()
{
	// 1. �÷��̾ ���ϴ� ������ ������
	FVector dir = target->GetActorLocation() - myActor->GetActorLocation();
	
	// ó�� ��ġ�� ���� ��ġ�� �Ÿ�
	float distance = FVector::Distance(originPos, myActor->GetActorLocation());
	// ���࿡ distance �� moveRange ���� Ŀ���� 
	if (distance > moveRange)
	{
		// Return ���·� ��ȯ
		ChangeState(EEnemyState::RETURN);
	}
	else
	{
		// 2. target ��ġ�� �������� (Navigation  ����� ���ؼ�)
		ai->MoveToLocation(target->GetActorLocation());

		//// 2. �� �������� ��������. 
		//myActor->AddMovementInput(dir.GetSafeNormal());

		// 3. �÷��̾���� �Ÿ��� ���� �������� ������
		float dist = dir.Length();
		if (dist < attackRange)
		{
			// 4. ���� ���¸� ATTACK �� �ٲ���
			ChangeState(EEnemyState::ATTACK);
		}
	}
}

void UEnemyFSM::UpdatePatrol()
{
	// �� ��ġ�� �����ϰ� ���� ��ġ�� �Ÿ��� ���Ѵ�.
	float dist = FVector::Distance(patrolPos, myActor->GetActorLocation());

	// �� �Ÿ��� 0�̸�
	if (dist < 50)
	{
		// IDLE ���·� ��ȯ
		ChangeState(EEnemyState::IDLE);
	}
}

void UEnemyFSM::UpdateAttack()
{
	ChangeState(EEnemyState::ATTACK_DELAY);
}

void UEnemyFSM::UpdateAttackDelay()
{	
	//// 1. �ð��� �帣�� �Ѵ�.
	//currTime += GetWorld()->DeltaTimeSeconds;
	//// 2. ���࿡ Attack Delay �ð��� ������
	//if (currTime > attackDelayTime)

	if (IsWaitComplete(attackDelayTime))
	{
		// �÷������ �Ÿ�
		float dist = FVector::Distance(target->GetActorLocation(), myActor->GetActorLocation());
		// �� �Ÿ��� ���ݹ���- > ��¥ ����
		if (dist < attackRange)
		{
			// 3. ���� ���·� ����
			ChangeState(EEnemyState::ATTACK);
		}
		// �������� -> �̵� 
		else if (CanTrace())
		{
			ChangeState(EEnemyState::MOVE);
		}
		// �� �ܴ� -> ���
		else
		{
			ChangeState(EEnemyState::IDLE);
		}

		// 4. ����ð� �ʱ�ȭ
		currTime = 0;
	}
}

void UEnemyFSM::UpdateDamaged(float deltaTime)
{
	if (IsWaitComplete(damageDelayTime))
	{
		// IDLE ���·� ��ȯ
		ChangeState(EEnemyState::IDLE);
	}

	//// damageDelayTime ��ٷȴٰ�
	//currTime += deltaTime;
	//if (currTime > damageDelayTime)
	//{
	//	// IDLE ���·� ��ȯ
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

void UEnemyFSM::UpdateReturn()
{
	EPathFollowingRequestResult::Type result = ai->MoveToLocation(originPos);
	if (result == EPathFollowingRequestResult::Type::AlreadyAtGoal)
	{
		// IDLE ���·� ��ȯ
		ChangeState(EEnemyState::IDLE);
	}

	//// �� ��ġ�� ó�� ��ġ�� �Ÿ��� ���Ѵ�.
	//float dist = FVector::Distance(originPos, myActor->GetActorLocation());
	////UE_LOG(LogTemp, Warning, TEXT("dist : %f"), dist);
	//// �� �Ÿ��� 0�̸�
	//if (dist < 50)
	//{
	//	// IDLE ���·� ��ȯ
	//	ChangeState(EEnemyState::IDLE);
	//}
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
	// 1. �÷��̾���� �Ÿ��� ������.
	FVector dir = target->GetActorLocation() - myActor->GetActorLocation();
	float dist = dir.Length();
	// 2. ���࿡ �Ÿ��� �������� ���� ������
	if (dist < traceRange)
	{
		// ���� �չ���� �÷�� ���ϴ� ������ ����
		float dot = FVector::DotProduct(dir.GetSafeNormal(), myActor->GetActorForwardVector());

		if (dot > viewAngle)
		{
			// �÷��̾ ���ϴ� ���⿡ ��ֹ��� �ִ��� �Ǵ� (LineTrace)
			FVector start = myActor->GetActorLocation();
			FVector end = target->GetActorLocation();
			FHitResult hitInfo;
			FCollisionQueryParams param;
			param.AddIgnoredActor(myActor);


			bool hit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, param);

			if (hit)
			{
				if (hitInfo.GetActor()->GetName().Contains(TEXT("Player")) == false)
				{
					return false;
				}
			}			

			return true;
		}
		
		//// ������ ���� acos ����
		//float radianAngle = FMath::Acos(dot);
		//// degree ������ ��������
		//float degreeAngle = FMath::RadiansToDegrees(radianAngle);
		//// ���࿡ �÷��̾ �þఢ(60)�� ���Դٸ� (degreeAngle <= 60 / 2) 
		//if (degreeAngle <= 180 * 0.5f)
		//{
		//	return true;
		//}
	}

	return false;
}

