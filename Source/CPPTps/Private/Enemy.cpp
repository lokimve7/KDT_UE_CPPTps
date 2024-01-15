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

	

	// Skeletal Mesh 읽어오자
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Enemy/Ch03_nonPBR.Ch03_nonPBR'"));
	if (tempMesh.Succeeded())
	{
		// Mesh 에 Skeletal Mesh 셋팅
		GetMesh()->SetSkeletalMesh(tempMesh.Object);	
	}

	// 메쉬의 위치/회전 값을 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));


	// FSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	// 이동 방향으로 나를 회전 시켜줘 옵션 활성화
	GetCharacterMovement()->bOrientRotationToMovement = true;



	// Capsule 컴포넌트 CollisonPreset = EnemyProfile
	// Mesh 컴포넌트 CollisionPreset = NoCollision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 현재 HP 를 maxHP 로 하자
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
	// HP 를 줄이자
	currHP -= damage;
	// 만약에 HP 가 0보다 크면
	if (currHP > 0)
	{
		// DAMAGE 상태로 전환
		fsm->ChangeState(EEnemyState::DAMAGE);
	}
	// 그렇지 않으면 (HP 가 0)
	else
	{
		// DIE 로 상태로 전환
		fsm->ChangeState(EEnemyState::DIE);
	}
}

