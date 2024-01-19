// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// Enemy ���¸� ���� (enum ������)
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVE,
	PATROL,
	RETURN,
	ATTACK,
	ATTACK_DELAY,
	DAMAGE,
	DIE
};

// Enemy ���� ��� ����
UENUM(BlueprintType)
enum class EAttackType : uint8
{
	KICK,
	PUNCH
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPTPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ���� ���� ���¸� ���� ����
	EEnemyState currState = EEnemyState::IDLE;

	// Ÿ�� (Player)
	UPROPERTY()
	class ATpsPlayer* target;

	// �� ������Ʈ�� � ���Ϳ� �پ��ִ���
	UPROPERTY()
	class AEnemy* myActor;

	// ������ �����Ǿ� �ִ� Animation class ������ ����
	UPROPERTY()
	class UAnimEnemy* anim;

	// Enemy Montage ���� ����
	UPROPERTY(EditAnywhere)
	class UAnimMontage* montage;

	// �Ѿ� �� �� �ִ� ���� (���� ����)
	UPROPERTY(EditAnywhere)
	float traceRange = 500;

	// ���� �� �� �ִ� ���� (���� ����)
	UPROPERTY(EditAnywhere)
	float attackRange = 200;

	// ���� ������ �� �ִ� ����
	UPROPERTY(EditAnywhere)
	float moveRange = 1000;

	// ����ð�
	float currTime = 0;
	// ���ݴ��ð�
	UPROPERTY(EditAnywhere)
	float attackDelayTime = 2;
	// �ǰݴ��ð�
	UPROPERTY(EditAnywhere)
	float damageDelayTime = 2;
	// IDLE ���� ��� �ð�
	UPROPERTY(EditAnywhere)
	float idleDelayTime = 2;

	// AI Controller
	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	// �þ߰� ( cos(�þ߰�) )
	float viewAngle = 180;

	// ���� ó�� ��ġ
	FVector originPos;
	// Patrol �ؾ��ϴ� ������ ��ġ
	FVector patrolPos;

public:
	// ���°� �ٲ� �ѹ� ���� �Ǵ� �Լ�
	void ChangeState(EEnemyState s);
	// ���
	void UpdateIdle();
	// �̵�
	void UpdateMove();
	// ��Ʈ�� 
	void UpdatePatrol();
	// ����
	void UpdateAttack();
	// ���� ���
	void UpdateAttackDelay();
	// �ǰ�
	void UpdateDamaged(float deltaTime);
	// ����
	void UpdateDie();
	// ����
	void UpdateReturn();

	// �ð� üũ �Լ�
	bool IsWaitComplete(float delay);

	// �÷��̾ �Ѿ� �� �� �ִ� ��Ȳ����
	bool CanTrace();
};
