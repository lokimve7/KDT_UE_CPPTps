// Fill out your copyright notice in the Description page of Project Settings.


#include "TpsPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <InputMappingContext.h>
#include <InputAction.h>
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include "Bullet.h"
#include <Kismet/KismetMathLibrary.h>
#include <Particles/ParticleSystem.h>
#include "SniperWidget.h"
#include "Inventory.h"
#include "TpsGameInstance.h"
#include "Enemy.h"
#include "EnemyFSM.h"
#include "InvenWidget.h"
#include "InvenItem.h"
#include <Components/CapsuleComponent.h>



// Sets default values
ATpsPlayer::ATpsPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// imc default 파일 읽어오자
	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempImc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (tempImc.Succeeded())
	{
		imcDefault = tempImc.Object;
	}

	// ia_jump 파일 읽어오자
	ConstructorHelpers::FObjectFinder<UInputAction> tempIAJump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (tempIAJump.Succeeded())
	{
		ia_Jump = tempIAJump.Object;
	}

	// ia_Fire 파일 읽어오자
	ConstructorHelpers::FObjectFinder<UInputAction> tempIAFire(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Fire.IA_Fire'"));
	if (tempIAFire.Succeeded())
	{
		ia_Fire = tempIAFire.Object;
	}

	// ia_Zoom 파일 읽어오자
	ConstructorHelpers::FObjectFinder<UInputAction> tempIAZoom(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Zoom.IA_Zoom'"));
	if (tempIAZoom.Succeeded())
	{
		ia_Zoom = tempIAZoom.Object;
	}

	// Bullet 블루프린트 가져오자
	ConstructorHelpers::FClassFinder<ABullet> tempBullet(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded())
	{
		bulletFactory = tempBullet.Class;
	}

	// Impact 효과 가져오자
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Metal_Large_01.P_Impact_Metal_Large_01'"));
	if (tempEffect.Succeeded())
	{
		impactEffect = tempEffect.Object;
	}

	// Skeletal Mesh 읽어오자
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (tempMesh.Succeeded())
	{
		// Mesh 에 Skeletal Mesh 셋팅
		//USkeletalMeshComponent* mesh = GetMesh();
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	// Mesh 의 위치 값과 회전값을 셋팅
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); // Pitch, Yaw, Roll

	//Spring Arm 컴포넌트 생성
	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ARM"));
	// arm 을 rootComponent 의 자식
	arm->SetupAttachment(RootComponent);
	// arm 위치를 바꾸자
	arm->SetRelativeLocation(FVector(0, 70, 90));

	// Camera 컴포넌트 생성
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAM"));
	// cam 을 arm 의 자식으로 셋팅
	cam->SetupAttachment(arm);

	// gun 컴포넌트 생성
	gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GUN"));
	gun->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	gun->SetRelativeLocation(FVector(-6.31f, 4.85f, -3.4f));
	//(Pitch = 0.000000, Yaw = -90.000000, Roll = 0.000000)
	//pitch yaw roll
	gun->SetRelativeRotation(FRotator(0, 90, 0));

	// gun 모양 읽어오자
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Assault_Rifle_A.Assault_Rifle_A'"));
	if (tempGun.Succeeded())
	{
		gun->SetSkeletalMesh(tempGun.Object);
	}

	// sniper 컴포넌트 생성
	sniper = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SNIPER"));
	sniper->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	sniper->SetRelativeLocation(FVector(-6.31f, 4.85f, -3.4f));
	sniper->SetRelativeRotation(FRotator(0, 90, 0));
	// sniper 모양 읽어오자
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSniper(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Sniper_Rifle_A.Sniper_Rifle_A'"));
	if (tempSniper.Succeeded())
	{
		sniper->SetSkeletalMesh(tempSniper.Object);
	}

	// Sniper Widget 블루프린트 가져오자
	ConstructorHelpers::FClassFinder<USniperWidget> tempSniperWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/BP_SniperWidget.BP_SniperWidget_C'"));
	if (tempSniperWidget.Succeeded())
	{
		sniperWidget = tempSniperWidget.Class;
	}

	// 총 쏘는 몽타주 가져오자
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Fire_Montage.Fire_Montage'"));
	if (tempMontage.Succeeded())
	{
		fireMontage = tempMontage.Object;
	}


	// collision preset 을 PlayerProfile 로 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));


//-------------------------------------------
	compInven = CreateDefaultSubobject<UInventory>(TEXT("INVEN"));
}

// Called when the game starts or when spawned
void ATpsPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 움직이는 속력을 walkSpeed 로 하자
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	// Controller 의 회전값을 따라 갈지 여부
	bUseControllerRotationYaw = true;
	arm->bUsePawnControlRotation = true;

	// 카메라 상/하 회전값 을 제한 (min, max 설정)
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	playerCon->PlayerCameraManager->ViewPitchMin = -60;
	playerCon->PlayerCameraManager->ViewPitchMax = 60;

	/*APlayerCameraManager* camManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	camManager->ViewPitchMin = -60;
	camManager->ViewPitchMax = 60;*/

	// 점프 횟수 제한
	JumpMaxCount = 3;

	// APlayerController 가져오자
	APlayerController* playerController = Cast<APlayerController>(GetController());
	//subSystem 을 가져오자	
	auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	// imcDefault 추가 하자
	subSystem->AddMappingContext(imcDefault, 0);

	// sniper widget 생성
	sniperUI = CreateWidget<USniperWidget>(GetWorld(), sniperWidget);

	// currWeaponMode 의 값에 따라서 무기를 선택하자
	ChangeWeapon(currWeaponMode);
	

	// 인벤토리 Widget 생성
	inven = CreateWidget<UInvenWidget>(GetWorld(), invenFactory);

	// 현재 HP 를 MAX HP 로 하자
	currHP = maxHP;
 }

// Called every frame
void ATpsPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// MoveAction();
	// RotateAction();	
}

// Called to bind functionality to input
void ATpsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		input->BindAction(ia_Jump, ETriggerEvent::Started, this, &ATpsPlayer::EnhancedJump);

		input->BindAction(ia_MouseMove, ETriggerEvent::Triggered, this, &ATpsPlayer::EnhancedMouse);

		input->BindAction(ia_Move, ETriggerEvent::Triggered, this, &ATpsPlayer::EnhancedMove);

		input->BindAction(ia_Fire, ETriggerEvent::Started, this, &ATpsPlayer::EnhancedFire);

		input->BindAction(ia_Zoom, ETriggerEvent::Triggered, this, &ATpsPlayer::EnhancedZoom);

		input->BindAction(ia_RealFire, ETriggerEvent::Triggered, this, &ATpsPlayer::EnhancedRealFire);

		input->BindAction(ia_Run, ETriggerEvent::Triggered, this, &ATpsPlayer::EnhancedRun);


		//---------------------
		input->BindAction(ia_GetItem, ETriggerEvent::Triggered, this, &ATpsPlayer::InputGetItem);

		input->BindAction(ia_OnOffInven, ETriggerEvent::Triggered, this, &ATpsPlayer::InputOnOffInventory);
	}

	PlayerInputComponent->BindAction(TEXT("RMouseClick"), IE_Pressed, this, &ATpsPlayer::InputRMouseClick);
	PlayerInputComponent->BindAction(TEXT("RMouseClick"), IE_Released, this, &ATpsPlayer::InputMouseUp);

	PlayerInputComponent->BindAction(TEXT("LMouseClick"), IE_Pressed, this, &ATpsPlayer::InputLMouseClick);
	PlayerInputComponent->BindAction(TEXT("LMouseClick"), IE_Released, this, &ATpsPlayer::InputMouseUp);

	PlayerInputComponent->BindAction(TEXT("ItemSort"), IE_Pressed, this, &ATpsPlayer::InputSort);


}

void ATpsPlayer::DamageProcess(float damage)
{
	currHP -= damage;
	UE_LOG(LogTemp, Warning, TEXT("curr hp : %f"), currHP);
	// HP Widget 갱신

	if (currHP <= 0)
	{
		// Game Over 처리		
	}
}

void ATpsPlayer::MoveAction(FVector2d keyboardInput)
{
	//// p = p0 + vt
	//FVector p0 = GetActorLocation();
	//FVector dir = GetActorRightVector() * moveInput.Y +
	//	GetActorForwardVector() * moveInput.X;
	//// dir 의 크기를 1로 만든다.
	//dir.Normalize();
	//// vt 만큼 움직여라
	//FVector vt = dir * moveSpeed * DeltaTime;
	//// 새로 이동해야 하는 위치
	//FVector p = p0 + vt;
	//SetActorLocation(p);

	FVector dir = GetActorRightVector() * keyboardInput.X +
		GetActorForwardVector() * keyboardInput.Y;
	// dir 의 크기를 1로 만든다.
	dir.Normalize();

	// dir 방향으로 움직여라
	AddMovementInput(dir);
}

void ATpsPlayer::RotateAction()
{
	// 나의 회전 yaw (z 축) 값 셋팅
	SetActorRotation(FRotator(0, mx, 0));
	// spring arm 의 회전 pitch (y 축) 값 셋팅
	arm->SetRelativeRotation(FRotator(my, 0, 0));
}

void ATpsPlayer::ChangeWeapon(EWeaponType weaponIdx)
{
	// 나의 현재 무기는 weaponIdx 다!! 설정
	currWeaponMode = weaponIdx;

	switch (weaponIdx)
	{
	// 만약에 weaponIdx 가 1이면 
	case EWeaponType::GUN:
		// Gun 을 보이게 하고, Sniper 를 보이지 않게
		gun->SetVisibility(true);
		sniper->SetVisibility(false);
		// 만약에 Sniper UI 가 화면에 있다면
		if (sniperUI->IsInViewport())
		{
			// 제거해라 / 화면 축소
			ZoomInOut(false);
		}
		break;
	// 만약에 weaponIdx 가 2이면 
	case EWeaponType::SNIPER:
		// Gun 을 보이지 않게 하고, Sniper 를 보이게
		gun->SetVisibility(false);
		sniper->SetVisibility(true);
		break;
	default:
		break;
	}	
}

void ATpsPlayer::ZoomInOut(bool isPressed)
{
	// 1. 만약에 value 의 값이 true (마우스 오른쪽 버튼을 눌렀다면)
	if (isPressed)
	{
		// 2. Sniper UI 을 화면에 붙이자.
		sniperUI->AddToViewport();
		// 화면 확대
		cam->SetFieldOfView(45);
	}
	// 3. 그렇지 않고 value 의 값이 false (마우스 왼쪽 버튼을 떼었다면)
	else
	{
		// 4. Sniper UI 을 화면에에서 지우자
		sniperUI->RemoveFromParent();
		// 화면 축소
		cam->SetFieldOfView(90);
	}
}

void ATpsPlayer::EnhancedJump()
{
	Jump();
}

void ATpsPlayer::EnhancedMouse(const FInputActionValue& value)
{
	FVector2d mouseValue = value.Get<FVector2d>();

	AddControllerYawInput(mouseValue.X);
	AddControllerPitchInput(mouseValue.Y);
}

void ATpsPlayer::EnhancedMove(const FInputActionValue& value)
{
	FVector2d keyboardValue = value.Get<FVector2d>();

	MoveAction(keyboardValue);
}

void ATpsPlayer::EnhancedFire(const FInputActionValue& value)
{
	int32 actionValue = value.Get<float>();

	EWeaponType type = (EWeaponType)actionValue;

	ChangeWeapon(type);
}

void ATpsPlayer::EnhancedZoom(const struct FInputActionValue& value)
{
	// 만약에 너의 현재 무기가 sniper 가 아니라면
	// 함수를 나가라
	if (currWeaponMode != EWeaponType::SNIPER) return;

	bool isPressed = value.Get<bool>();
	
	ZoomInOut(isPressed);
}

void ATpsPlayer::EnhancedRealFire()
{
	// 총 쏘는 애니메이션을 하자
	UAnimInstance* animPlayer = GetMesh()->GetAnimInstance();
	animPlayer->Montage_Play(fireMontage);
	// 만약에 걷고 있다면
	if (isRun == false)
	{
		// Fire01 로 Montage 점프
		animPlayer->Montage_JumpToSection(TEXT("Fire1"));
	}
	// 그렇지 않고 뛰고 있다면
	else
	{
		// 만약에 점프 중이라면
		if (GetCharacterMovement()->IsFalling())
		{
			// Fire01 로 Montage 점프
			animPlayer->Montage_JumpToSection(TEXT("Fire1"));
		}
		// 바닥에 있다면
		else
		{
			// FIre02 로 Montage 점프
			animPlayer->Montage_JumpToSection(TEXT("Fire2"));
		}
	}


	switch (currWeaponMode)
	{
		case EWeaponType::GUN:
		{
			// 생성되야하는 위치 계산 (나의 위치 + 나의 앞방향으로 100만큼 떨어진 값)
			//FVector pos = GetActorLocation() + GetActorForwardVector() * 100;
			FVector pos = gun->GetSocketLocation(TEXT("FirePos"));
			FRotator rot = gun->GetSocketRotation(TEXT("FirePos"));
			// 총알 공장을 이용해서 총알을 만든다. ( with 위치, 회전)
			GetWorld()->SpawnActor<ABullet>(bulletFactory, pos, rot);
		}
		break;

		case EWeaponType::SNIPER:
		{
			// LineTrace 시작 지점
			FVector start = cam->GetComponentLocation();
			// LineTrace 끝 지점
			FVector end = start + cam->GetForwardVector() * 5000;
			// 어딘가 부딪혔을 때 부딪힌 Actor 정보를 담을 변수 
			FHitResult hitInfo;
			// 충돌 옵션 설정
			FCollisionQueryParams param;
			param.AddIgnoredActor(this);

			// 책에 나와있는 내용(UKismetSystemLibrary::LineTraceSingle 안에 구현 되어있는 방법)
			bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, param);


			// 블루 프린트에 사용하는 노드
			/*TArray<AActor*> ignoreActor;			
			bool isHit = UKismetSystemLibrary::LineTraceSingle(
				GetWorld(), 
				start, 
				end, 
				UEngineTypes::ConvertToTraceType(ECC_Visibility), 
				false, 
				ignoreActor, 
				EDrawDebugTrace::None, 
				hitInfo, 
				true);*/

			// 만약에 LineTrace 가 어딘가에 부딪혔다면
			if (isHit)
			{
				// 효과의 회전값을 부딪힌 곳의 수직벡터(NormalVector) 를 이용해서 계산하자
				FRotator rot = UKismetMathLibrary::MakeRotFromX(hitInfo.ImpactNormal);

				// impact 효과를 보여주자
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), 
					impactEffect, 
					hitInfo.ImpactPoint,
					rot);								

				// 맞은놈이 Enemy 라면 DAMAGE 상태로 전환
				if (hitInfo.GetComponent()->GetCollisionObjectType() == ECC_GameTraceChannel1)
				{
					AActor* hitActor = hitInfo.GetActor();
					AEnemy* enemy = Cast<AEnemy>(hitActor);
					enemy->DamageProcess(1);					
				}
			}
		}
		break;
	}
}

void ATpsPlayer::EnhancedRun(const struct FInputActionValue& value)
{
	bool actionValue = value.Get<bool>();

	// 걷기 상태인지 달리기 상태인지 구분
	isRun = actionValue;

	if (actionValue)
	{
		// 달리기 모드
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
	else
	{
		// 걷기 모드
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
}

void ATpsPlayer::InputGetItem(const struct FInputActionValue& value)
{
	int32 actionValue = value.Get<float>() ;
	actionValue--;
	//actionValue 해당 되는 아이템을 추가( compInven->myItems 에)
	UTpsGameInstance* gameInstance = Cast<UTpsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	compInven->myItems.Add(gameInstance->defineItem[actionValue]);

	// 만약에 Inven 이 열려있다면 
	if (inven->IsInViewport())
	{
		// 추가 되는 아이템이 몇 번째에 있는지
		int32 idx = compInven->myItems.Num() - 1;

		// Inven 에 아이템이 추가된 것을 보여주자
		inven->CreateItem(compInven->myItems[idx], idx);
	}
}

void ATpsPlayer::InputOnOffInventory()
{
	// 만약에 Inven 이 화면에 붙어있으면 지우고
	if (inven->IsInViewport())
	{
		inven->RemoveFromParent();
		// 마우스 포인터 비활성
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	}
	// 그렇지 않으면 화면에 붙이자
	else
	{
		inven->AddToViewport();
		// 인벤토리 새로고침
		inven->RefreshInven(compInven->myItems);
		// 마우스 포인터 활성화
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);

	}
}

void ATpsPlayer::OnHoverItem(class UInvenItem* invenItem)
{
	onHoverItem = invenItem;	
}

void ATpsPlayer::InputRMouseClick()
{
	if(onHoverItem ==nullptr) return;

	// 해당 아이템 사용	
	compInven->myItems.RemoveAt(onHoverItem->idxInInven);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *onHoverItem->itemData.name);

	inven->RefreshInven(compInven->myItems);
}

void ATpsPlayer::InputMouseUp()
{
	if(onHoverItem == nullptr) return;

	int32 dest = onHoverItem->EndMove();

	// 인벤 밖에 놓았을때
	if (dest == -1)
	{
		onHoverItem->SetPostion();
	}
	// 인벤 안이긴 하지만 아이템이 존재하지 않은 위치에 놓았을 때
	else if (dest >= compInven->myItems.Num())
	{
		onHoverItem->SetPostion();
	}
	// 다른 아이템 위치에 놓았을 때
	else
	{
		// myItems 데이터를 교환
		// dest : onHoverItem 이 이동해야하는 Inven 위치(idx)
		// sour : onHoverItem 이 현재 Inven 있는 위치
		int32 sour = onHoverItem->idxInInven;

		FItemData temp = compInven->myItems[sour];
		compInven->myItems[sour] = compInven->myItems[dest];
		compInven->myItems[dest] = temp;

		// InvenWidget 에 있는 allItemWidget 데이터 교환 (위치도 교환)
		inven->SwitchItem(dest, sour);
		//inven->RefreshInven(compInven->myItems);
	}


	onHoverItem = nullptr;
}

void ATpsPlayer::InputLMouseClick()
{
	if(onHoverItem == nullptr) return;
	onHoverItem->BeginMove();
}

void ATpsPlayer::InputSort()
{
	compInven->myItems.Sort(FItemSortByType());
	inven->RefreshInven(compInven->myItems);
}

