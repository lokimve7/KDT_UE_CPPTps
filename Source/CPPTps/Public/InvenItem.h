// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.h"
#include "InvenItem.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHover, class UInvenItem*, invenItem);

UCLASS()
class CPPTPS_API UInvenItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* thumbnail;

	UPROPERTY()
	FItemData itemData;

	int32 idxInInven = -1;

	// 마우스 클릭 시 마우스 커서가 내 위에 있으면
	// onHover 에 등록된 함수를 호출 용도
	UPROPERTY()
	FOnHover onHover;
		

public:
	void InitItem(FItemData data, int32 idx, UObject* InObject, const FName& InFunctionName);
};
