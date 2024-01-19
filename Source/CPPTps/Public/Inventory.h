// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/DataTable.h>
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
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	// ������ ����
	UPROPERTY(EditAnywhere)
	EItemType type;
	// ������ ����� (�κ��丮���� �������� �̹���)
	UPROPERTY(EditAnywhere)
	UTexture2D* thumbnail;
	// ������ �̸�
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
	// ���� ��� �ִ� �����۵�
	UPROPERTY(EditAnywhere)
	TArray<FItemData> myItems;
};
