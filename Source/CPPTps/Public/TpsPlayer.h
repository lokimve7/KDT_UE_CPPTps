// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "TpsPlayer.generated.h"

UCLASS()
class CPPTPS_API ATpsPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATpsPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* arm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* cam;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gun;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* sniper;

	float moveSpeed = 500;

	// 선택한 무기 
	int32 currWeaponMode = 1;

	// 회전 값
	float mx = 0;
	float my = 0;

	// Input Mapping Context 담을 변수
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imcDefault;

	// InputAction Jump
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Jump;

	// InputAction Mouse
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_MouseMove;

	// InputAction Move (W, A, S, D)
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Move;

	// InputAction Fire 
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Fire;

	// InputAction Zoom
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Zoom;

	// InputAction RealFire
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_RealFire;

	// 총알 Blueprint (총알공장)
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	// 파티클 효과 ( LineTrace 에 부딪힌 곳에 보여주자)
	UPROPERTY(EditAnywhere)
	class UParticleSystem* impactEffect;

	// Sniper Widget 블루프린트 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USniperWidget> sniperWidget;
	// Sniper Widget 을 통해 만든 UI 를 담을 변수
	UPROPERTY(EditAnywhere)
	class USniperWidget* sniperUI;

public:

	void MoveAction(FVector2d keyboardInput);
	void RotateAction();
	void ChangeWeapon(int32 weaponIdx);
	void ZoomInOut(bool isPressed);

	void EnhancedJump();
	void EnhancedMouse(const struct FInputActionValue& value);
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedFire(const struct FInputActionValue& value);
	void EnhancedZoom(const struct FInputActionValue& value);
	void EnhancedRealFire();
};

