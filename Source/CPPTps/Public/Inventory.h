// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	ITEM_SPHERE,
	ITEM_CUBE,
	ITEM_CAPSULE,
	ITME_MAX
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
public:
	// 아이템 종류
	UPROPERTY(EditAnywhere)
	EItemType type;
	// 아이템 썸네일 (인벤토리에서 보여지는 이미지)
	UPROPERTY(EditAnywhere)
	UTexture2D* thumbnail;
	// 아이템 이름
	UPROPERTY(EditAnywhere)
	FString name;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CPPTPS_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 내가 들고 있는 아이템들
	UPROPERTY(EditAnywhere)
	TArray<FItemData> myItems;
};
