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

	// imc default ���� �о����
	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempImc(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (tempImc.Succeeded())
	{
		imcDefault = tempImc.Object;
	}

	// ia_jump ���� �о����
	ConstructorHelpers::FObjectFinder<UInputAction> tempIAJump(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (tempIAJump.Succeeded())
	{
		ia_Jump = tempIAJump.Object;
	}

	// ia_Fire ���� �о����
	ConstructorHelpers::FObjectFinder<UInputAction> tempIAFire(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Fire.IA_Fire'"));
	if (tempIAFire.Succeeded())
	{
		ia_Fire = tempIAFire.Object;
	}

	// ia_Zoom ���� �о����
	ConstructorHelpers::FObjectFinder<UInputAction> tempIAZoom(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Zoom.IA_Zoom'"));
	if (tempIAZoom.Succeeded())
	{
		ia_Zoom = tempIAZoom.Object;
	}

	// Bullet �������Ʈ ��������
	ConstructorHelpers::FClassFinder<ABullet> tempBullet(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded())
	{
		bulletFactory = tempBullet.Class;
	}

	// Impact ȿ�� ��������
	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("/Script/Engine.ParticleSystem'/Game/MilitaryWeapSilver/FX/P_Impact_Metal_Large_01.P_Impact_Metal_Large_01'"));
	if (tempEffect.Succeeded())
	{
		impactEffect = tempEffect.Object;
	}

	// Skeletal Mesh �о����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (tempMesh.Succeeded())
	{
		// Mesh �� Skeletal Mesh ����
		//USkeletalMeshComponent* mesh = GetMesh();
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	// Mesh �� ��ġ ���� ȸ������ ����
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); // Pitch, Yaw, Roll

	//Spring Arm ������Ʈ ����
	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ARM"));
	// arm �� rootComponent �� �ڽ�
	arm->SetupAttachment(RootComponent);
	// arm ��ġ�� �ٲ���
	arm->SetRelativeLocation(FVector(0, 70, 90));

	// Camera ������Ʈ ����
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("CAM"));
	// cam �� arm �� �ڽ����� ����
	cam->SetupAttachment(arm);

	// gun ������Ʈ ����
	gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GUN"));
	gun->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	gun->SetRelativeLocation(FVector(-6.31f, 4.85f, -3.4f));
	//(Pitch = 0.000000, Yaw = -90.000000, Roll = 0.000000)
	//pitch yaw roll
	gun->SetRelativeRotation(FRotator(0, 90, 0));

	// gun ��� �о����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Assault_Rifle_A.Assault_Rifle_A'"));
	if (tempGun.Succeeded())
	{
		gun->SetSkeletalMesh(tempGun.Object);
	}

	// sniper ������Ʈ ����
	sniper = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SNIPER"));
	sniper->SetupAttachment(GetMesh(), TEXT("WeaponPos"));
	sniper->SetRelativeLocation(FVector(-6.31f, 4.85f, -3.4f));
	sniper->SetRelativeRotation(FRotator(0, 90, 0));
	// sniper ��� �о����
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempSniper(TEXT("/Script/Engine.SkeletalMesh'/Game/MilitaryWeapSilver/Weapons/Sniper_Rifle_A.Sniper_Rifle_A'"));
	if (tempSniper.Succeeded())
	{
		sniper->SetSkeletalMesh(tempSniper.Object);
	}

	// Sniper Widget �������Ʈ ��������
	ConstructorHelpers::FClassFinder<USniperWidget> tempSniperWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/BP_SniperWidget.BP_SniperWidget_C'"));
	if (tempSniperWidget.Succeeded())
	{
		sniperWidget = tempSniperWidget.Class;
	}

	// �� ��� ��Ÿ�� ��������
	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("/Script/Engine.AnimMontage'/Game/Blueprints/Fire_Montage.Fire_Montage'"));
	if (tempMontage.Succeeded())
	{
		fireMontage = tempMontage.Object;
	}


	// collision preset �� PlayerProfile �� ����
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerProfile"));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));


//-------------------------------------------
	compInven = CreateDefaultSubobject<UInventory>(TEXT("INVEN"));
}

// Called when the game starts or when spawned
void ATpsPlayer::BeginPlay()
{
	Super::BeginPlay();

	// �����̴� �ӷ��� walkSpeed �� ����
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	// Controller �� ȸ������ ���� ���� ����
	bUseControllerRotationYaw = true;
	arm->bUsePawnControlRotation = true;

	// ī�޶� ��/�� ȸ���� �� ���� (min, max ����)
	APlayerController* playerCon = GetWorld()->GetFirstPlayerController();
	playerCon->PlayerCameraManager->ViewPitchMin = -60;
	playerCon->PlayerCameraManager->ViewPitchMax = 60;

	/*APlayerCameraManager* camManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	camManager->ViewPitchMin = -60;
	camManager->ViewPitchMax = 60;*/

	// ���� Ƚ�� ����
	JumpMaxCount = 3;

	// APlayerController ��������
	APlayerController* playerController = Cast<APlayerController>(GetController());
	//subSystem �� ��������	
	auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	// imcDefault �߰� ����
	subSystem->AddMappingContext(imcDefault, 0);

	// sniper widget ����
	sniperUI = CreateWidget<USniperWidget>(GetWorld(), sniperWidget);

	// currWeaponMode �� ���� ���� ���⸦ ��������
	ChangeWeapon(currWeaponMode);
	

	// �κ��丮 Widget ����
	inven = CreateWidget<UInvenWidget>(GetWorld(), invenFactory);

	// ���� HP �� MAX HP �� ����
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
	// HP Widget ����

	if (currHP <= 0)
	{
		// Game Over ó��		
	}
}

void ATpsPlayer::MoveAction(FVector2d keyboardInput)
{
	//// p = p0 + vt
	//FVector p0 = GetActorLocation();
	//FVector dir = GetActorRightVector() * moveInput.Y +
	//	GetActorForwardVector() * moveInput.X;
	//// dir �� ũ�⸦ 1�� �����.
	//dir.Normalize();
	//// vt ��ŭ ��������
	//FVector vt = dir * moveSpeed * DeltaTime;
	//// ���� �̵��ؾ� �ϴ� ��ġ
	//FVector p = p0 + vt;
	//SetActorLocation(p);

	FVector dir = GetActorRightVector() * keyboardInput.X +
		GetActorForwardVector() * keyboardInput.Y;
	// dir �� ũ�⸦ 1�� �����.
	dir.Normalize();

	// dir �������� ��������
	AddMovementInput(dir);
}

void ATpsPlayer::RotateAction()
{
	// ���� ȸ�� yaw (z ��) �� ����
	SetActorRotation(FRotator(0, mx, 0));
	// spring arm �� ȸ�� pitch (y ��) �� ����
	arm->SetRelativeRotation(FRotator(my, 0, 0));
}

void ATpsPlayer::ChangeWeapon(EWeaponType weaponIdx)
{
	// ���� ���� ����� weaponIdx ��!! ����
	currWeaponMode = weaponIdx;

	switch (weaponIdx)
	{
	// ���࿡ weaponIdx �� 1�̸� 
	case EWeaponType::GUN:
		// Gun �� ���̰� �ϰ�, Sniper �� ������ �ʰ�
		gun->SetVisibility(true);
		sniper->SetVisibility(false);
		// ���࿡ Sniper UI �� ȭ�鿡 �ִٸ�
		if (sniperUI->IsInViewport())
		{
			// �����ض� / ȭ�� ���
			ZoomInOut(false);
		}
		break;
	// ���࿡ weaponIdx �� 2�̸� 
	case EWeaponType::SNIPER:
		// Gun �� ������ �ʰ� �ϰ�, Sniper �� ���̰�
		gun->SetVisibility(false);
		sniper->SetVisibility(true);
		break;
	default:
		break;
	}	
}

void ATpsPlayer::ZoomInOut(bool isPressed)
{
	// 1. ���࿡ value �� ���� true (���콺 ������ ��ư�� �����ٸ�)
	if (isPressed)
	{
		// 2. Sniper UI �� ȭ�鿡 ������.
		sniperUI->AddToViewport();
		// ȭ�� Ȯ��
		cam->SetFieldOfView(45);
	}
	// 3. �׷��� �ʰ� value �� ���� false (���콺 ���� ��ư�� �����ٸ�)
	else
	{
		// 4. Sniper UI �� ȭ�鿡���� ������
		sniperUI->RemoveFromParent();
		// ȭ�� ���
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
	// ���࿡ ���� ���� ���Ⱑ sniper �� �ƴ϶��
	// �Լ��� ������
	if (currWeaponMode != EWeaponType::SNIPER) return;

	bool isPressed = value.Get<bool>();
	
	ZoomInOut(isPressed);
}

void ATpsPlayer::EnhancedRealFire()
{
	// �� ��� �ִϸ��̼��� ����
	UAnimInstance* animPlayer = GetMesh()->GetAnimInstance();
	animPlayer->Montage_Play(fireMontage);
	// ���࿡ �Ȱ� �ִٸ�
	if (isRun == false)
	{
		// Fire01 �� Montage ����
		animPlayer->Montage_JumpToSection(TEXT("Fire1"));
	}
	// �׷��� �ʰ� �ٰ� �ִٸ�
	else
	{
		// ���࿡ ���� ���̶��
		if (GetCharacterMovement()->IsFalling())
		{
			// Fire01 �� Montage ����
			animPlayer->Montage_JumpToSection(TEXT("Fire1"));
		}
		// �ٴڿ� �ִٸ�
		else
		{
			// FIre02 �� Montage ����
			animPlayer->Montage_JumpToSection(TEXT("Fire2"));
		}
	}


	switch (currWeaponMode)
	{
		case EWeaponType::GUN:
		{
			// �����Ǿ��ϴ� ��ġ ��� (���� ��ġ + ���� �չ������� 100��ŭ ������ ��)
			//FVector pos = GetActorLocation() + GetActorForwardVector() * 100;
			FVector pos = gun->GetSocketLocation(TEXT("FirePos"));
			FRotator rot = gun->GetSocketRotation(TEXT("FirePos"));
			// �Ѿ� ������ �̿��ؼ� �Ѿ��� �����. ( with ��ġ, ȸ��)
			GetWorld()->SpawnActor<ABullet>(bulletFactory, pos, rot);
		}
		break;

		case EWeaponType::SNIPER:
		{
			// LineTrace ���� ����
			FVector start = cam->GetComponentLocation();
			// LineTrace �� ����
			FVector end = start + cam->GetForwardVector() * 5000;
			// ��� �ε����� �� �ε��� Actor ������ ���� ���� 
			FHitResult hitInfo;
			// �浹 �ɼ� ����
			FCollisionQueryParams param;
			param.AddIgnoredActor(this);

			// å�� �����ִ� ����(UKismetSystemLibrary::LineTraceSingle �ȿ� ���� �Ǿ��ִ� ���)
			bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility, param);


			// ��� ����Ʈ�� ����ϴ� ���
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

			// ���࿡ LineTrace �� ��򰡿� �ε����ٸ�
			if (isHit)
			{
				// ȿ���� ȸ������ �ε��� ���� ��������(NormalVector) �� �̿��ؼ� �������
				FRotator rot = UKismetMathLibrary::MakeRotFromX(hitInfo.ImpactNormal);

				// impact ȿ���� ��������
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(), 
					impactEffect, 
					hitInfo.ImpactPoint,
					rot);								

				// �������� Enemy ��� DAMAGE ���·� ��ȯ
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

	// �ȱ� �������� �޸��� �������� ����
	isRun = actionValue;

	if (actionValue)
	{
		// �޸��� ���
		GetCharacterMovement()->MaxWalkSpeed = runSpeed;
	}
	else
	{
		// �ȱ� ���
		GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
	}
}

void ATpsPlayer::InputGetItem(const struct FInputActionValue& value)
{
	int32 actionValue = value.Get<float>() ;
	actionValue--;
	//actionValue �ش� �Ǵ� �������� �߰�( compInven->myItems ��)
	UTpsGameInstance* gameInstance = Cast<UTpsGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	compInven->myItems.Add(gameInstance->defineItem[actionValue]);

	// ���࿡ Inven �� �����ִٸ� 
	if (inven->IsInViewport())
	{
		// �߰� �Ǵ� �������� �� ��°�� �ִ���
		int32 idx = compInven->myItems.Num() - 1;

		// Inven �� �������� �߰��� ���� ��������
		inven->CreateItem(compInven->myItems[idx], idx);
	}
}

void ATpsPlayer::InputOnOffInventory()
{
	// ���࿡ Inven �� ȭ�鿡 �پ������� �����
	if (inven->IsInViewport())
	{
		inven->RemoveFromParent();
		// ���콺 ������ ��Ȱ��
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	}
	// �׷��� ������ ȭ�鿡 ������
	else
	{
		inven->AddToViewport();
		// �κ��丮 ���ΰ�ħ
		inven->RefreshInven(compInven->myItems);
		// ���콺 ������ Ȱ��ȭ
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

	// �ش� ������ ���	
	compInven->myItems.RemoveAt(onHoverItem->idxInInven);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *onHoverItem->itemData.name);

	inven->RefreshInven(compInven->myItems);
}

void ATpsPlayer::InputMouseUp()
{
	if(onHoverItem == nullptr) return;

	int32 dest = onHoverItem->EndMove();

	// �κ� �ۿ� ��������
	if (dest == -1)
	{
		onHoverItem->SetPostion();
	}
	// �κ� ���̱� ������ �������� �������� ���� ��ġ�� ������ ��
	else if (dest >= compInven->myItems.Num())
	{
		onHoverItem->SetPostion();
	}
	// �ٸ� ������ ��ġ�� ������ ��
	else
	{
		// myItems �����͸� ��ȯ
		// dest : onHoverItem �� �̵��ؾ��ϴ� Inven ��ġ(idx)
		// sour : onHoverItem �� ���� Inven �ִ� ��ġ
		int32 sour = onHoverItem->idxInInven;

		FItemData temp = compInven->myItems[sour];
		compInven->myItems[sour] = compInven->myItems[dest];
		compInven->myItems[dest] = temp;

		// InvenWidget �� �ִ� allItemWidget ������ ��ȯ (��ġ�� ��ȯ)
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

