// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "TpsPlayer.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NONE = 0,
	GUN = 1,
	SNIPER
};

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

	float runSpeed = 500;
	float walkSpeed = 500;

	// 선택한 무기 (1 : 라이플, 2 : 스나이퍼)
	EWeaponType currWeaponMode = EWeaponType::GUN;

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

	// InputAction Run
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_Run;

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

	// 총 쏘는 몽타주
	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireMontage;

	// 걷기 / 뛰기
	bool isRun = false;

public:

	void MoveAction(FVector2d keyboardInput);
	void RotateAction();
	void ChangeWeapon(EWeaponType weaponIdx);
	void ZoomInOut(bool isPressed);

	void EnhancedJump();
	void EnhancedMouse(const struct FInputActionValue& value);
	void EnhancedMove(const struct FInputActionValue& value);
	void EnhancedFire(const struct FInputActionValue& value);
	void EnhancedZoom(const struct FInputActionValue& value);
	void EnhancedRealFire();
	void EnhancedRun(const struct FInputActionValue& value);








//---------------------------------------------------
public:
	UPROPERTY(EditAnywhere)
	class UInventory* compInven;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_GetItem;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_OnOffInven;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UInvenWidget> invenFactory;
	UPROPERTY(EditAnywhere)
	class UInvenWidget* inven;

	UPROPERTY()
	class UInvenItem* onHoverItem = nullptr;

public:
	void InputGetItem(const struct FInputActionValue& value);
	void InputOnOffInventory();

	UFUNCTION()
	void OnHoverItem(class UInvenItem* invenItem);

	UFUNCTION()
	void InputRMouseClick();
	UFUNCTION()
	void InputMouseUp();
};

