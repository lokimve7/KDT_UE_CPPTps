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

	float moveSpeed = 500;

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

	// 총알 Blueprint (총알공장)
	TSubclassOf<class ABullet> bulletFactory;

public:

	void MoveAction(FVector2d keyboardInput);
	void RotateAction();

	void EnhancedJump();
	void EnhancedMouse(const struct FInputActionValue& value);
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedFire();
};

