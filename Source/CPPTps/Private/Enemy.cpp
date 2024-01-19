// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	// Skeletal Mesh �о����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Enemy/Ch03_nonPBR.Ch03_nonPBR'"));
	if (tempMesh.Succeeded())
	{
		// Mesh �� Skeletal Mesh ����
		GetMesh()->SetSkeletalMesh(tempMesh.Object);	
	}

	// �޽��� ��ġ/ȸ�� ���� ����
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));


	// FSM ������Ʈ �߰�
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	// �̵� �������� ���� ȸ�� ������ �ɼ� Ȱ��ȭ
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Capsule ������Ʈ CollisonPreset = EnemyProfile
	// Mesh ������Ʈ CollisionPreset = NoCollision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//Auto Possess ai ���� (spawn, placed �Ѵ� �����ϰ�)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// ���� HP �� maxHP �� ����
	currHP = maxHP;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::DamageProcess(int32 damage)
{
	// HP �� ������
	currHP -= damage;
	// ���࿡ HP �� 0���� ũ��
	if (currHP > 0)
	{
		// DAMAGE ���·� ��ȯ
		fsm->ChangeState(EEnemyState::DAMAGE);
	}
	// �׷��� ������ (HP �� 0)
	else
	{
		// DIE �� ���·� ��ȯ
		fsm->ChangeState(EEnemyState::DIE);
	}
}

