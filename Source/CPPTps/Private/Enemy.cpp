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

	// Widget ������Ʈ �߰�
	comHpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("BP_BAR"));
	comHpBar->SetupAttachment(RootComponent);
	comHpBar->SetDrawSize(FVector2D(100, 30));
	comHpBar->SetRelativeLocation(FVector(0, 0, 106));

	// HP Bar Widget �ҷ��ͼ� ����
	ConstructorHelpers::FClassFinder<UEnemyHPBar> tempHPBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/BP_EnemyHPBar.BP_EnemyHPBar_C'"));
	if (tempHPBar.Succeeded())
	{
		comHpBar->SetWidgetClass(tempHPBar.Class);
	}



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

	if (fsm->target != nullptr)
	{
		// ī�޶��� �չ���
		FVector camForward = fsm->target->cam->GetForwardVector();
		// ī�޶��� ������
		FVector camUp = fsm->target->cam->GetUpVector();
		// ī�޶��� �չ��� �ݴ�, �������� ���ϴ� ������ �����ּ���
		FRotator rot = UKismetMathLibrary::MakeRotFromXZ(-camForward, camUp);
		// rot �� ������ comHPBar �� ����(����)�� ����
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
	// HP �� ������
	currHP -= damage;

	// HPBar progress Percent �� ���� (0 ~ 1)
	UUserWidget* widget = comHpBar->GetWidget();
	UEnemyHPBar* hpBarWidget = Cast<UEnemyHPBar>(widget);	
	hpBarWidget->HPBar->SetPercent((float)currHP / maxHP);

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

void AEnemy::AttackTarget()
{
	fsm->target->DamageProcess(20);
}

