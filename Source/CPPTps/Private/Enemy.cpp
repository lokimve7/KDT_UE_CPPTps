// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "EnemyFSM.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/WidgetComponent.h>
#include "EnemyHPBar.h"
#include "TpsPlayer.h"
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Components/ProgressBar.h>


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

	// Widget 컴포넌트 추가
	comHpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("BP_BAR"));
	comHpBar->SetupAttachment(RootComponent);
	comHpBar->SetDrawSize(FVector2D(100, 30));
	comHpBar->SetRelativeLocation(FVector(0, 0, 106));

	// HP Bar Widget 불러와서 셋팅
	ConstructorHelpers::FClassFinder<UEnemyHPBar> tempHPBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/BP_EnemyHPBar.BP_EnemyHPBar_C'"));
	if (tempHPBar.Succeeded())
	{
		comHpBar->SetWidgetClass(tempHPBar.Class);
	}



	// 이동 방향으로 나를 회전 시켜줘 옵션 활성화
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Capsule 컴포넌트 CollisonPreset = EnemyProfile
	// Mesh 컴포넌트 CollisionPreset = NoCollision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//Auto Possess ai 설정 (spawn, placed 둘다 동작하게)
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

	if (fsm->target != nullptr)
	{
		// 카메라의 앞방향
		FVector camForward = fsm->target->cam->GetForwardVector();
		// 카메라의 윗방향
		FVector camUp = fsm->target->cam->GetUpVector();
		// 카메라의 앞방향 반대, 윗방향을 향하는 각도를 구해주세요
		FRotator rot = UKismetMathLibrary::MakeRotFromXZ(-camForward, camUp);
		// rot 의 값으로 comHPBar 의 각도(월드)로 셋팅
		comHpBar->SetWorldRotation(rot);
	}	
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

	// HPBar progress Percent 값 설정 (0 ~ 1)
	UUserWidget* widget = comHpBar->GetWidget();
	UEnemyHPBar* hpBarWidget = Cast<UEnemyHPBar>(widget);	
	hpBarWidget->HPBar->SetPercent((float)currHP / maxHP);

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

void AEnemy::AttackTarget()
{
	fsm->target->DamageProcess(50);
}

