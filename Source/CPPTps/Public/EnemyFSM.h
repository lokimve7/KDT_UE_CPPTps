// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// Enemy 상태를 정의 (enum 열거형)
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	IDLE,
	MOVE,
	ATTACK,
	ATTACK_DELAY,
	DAMAGE,
	DIE
};

// Enemy 공격 모션 정의
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

	// 현재 나의 상태를 담을 변수
	EEnemyState currState = EEnemyState::IDLE;

	// 타겟 (Player)
	class ATpsPlayer* target;

	// 이 컴포넌트가 어떤 액터에 붙어있는지
	class AEnemy* myActor;

	// 나한테 설정되어 있는 Animation class 가져올 변수
	class UAnimEnemy* anim;

	// 쫓아 갈 수 있는 범위 (인지 범위)
	UPROPERTY(EditAnywhere)
	float traceRange = 1000;

	// 공격 할 수 있는 범위 (공격 범위)
	UPROPERTY(EditAnywhere)
	float attackRange = 200;

	// 현재시간
	float currTime = 0;
	// 공격대기시간
	UPROPERTY(EditAnywhere)
	float attackDelayTime = 2;
	// 피격대기시간
	UPROPERTY(EditAnywhere)
	float damageDelayTime = 2;

public:
	// 상태가 바뀔때 한번 실행 되는 함수
	void ChangeState(EEnemyState s);
	// 대기
	void UpdateIdle();
	// 이동
	void UpdateMove();
	// 공격
	void UpdateAttack();
	// 공격 대기
	void UpdateAttackDelay();
	// 피격
	void UpdateDamaged(float deltaTime);
};
