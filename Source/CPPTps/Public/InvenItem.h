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

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	UPROPERTY(meta = (BindWidget))
	class UImage* thumbnail;

	UPROPERTY()
	FItemData itemData;

	// 인벤토리에서 몇번째 위치에 있는지 (내가 가지고 있는 아이템에서 몇번째 있는지)
	int32 idxInInven = -1;

	// 마우스 클릭 시 마우스 커서가 내 위에 있으면
	// onHover 에 등록된 함수를 호출 용도
	UPROPERTY()
	FOnHover onHover;
		
	// 마우스에 의해서 움직이는지
	bool isMove = false;

	// 마우스 왼쪽 클릭했을 때 얼마만큼 떨어져있는지
	FVector2D offsetMouse;


	class UCanvasPanelSlot* panelSlot;

public:
	void InitItem(FItemData data, int32 idx, UObject* InObject, const FName& InFunctionName);
	void BeginMove();
	int32 EndMove();
	void SetPostion();
};
