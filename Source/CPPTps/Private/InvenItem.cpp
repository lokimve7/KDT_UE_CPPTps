// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenItem.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>
#include <Blueprint/WidgetLayoutLibrary.h>

// 마우스 (왼쪽/오른쪽) 눌릴때 (마우스포인터가 내 위에 있을 때)
FReply UInvenItem::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	onHover.ExecuteIfBound(this);

	//UE_LOG(LogTemp, Warning, TEXT("item name : %s"), *itemData.name);

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UInvenItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// 마우스를 따라서 움직일 수 있다면
	if (isMove)
	{
		// 마우스 위치로 나의 위치를 셋팅
		FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		panelSlot->SetPosition(mousePos - offsetMouse);
	}
}

void UInvenItem::InitItem(FItemData data, int32 idx, UObject* InObject, const FName& InFunctionName)
{
	itemData = data;
	idxInInven = idx;

	// 인벤토리이에서 item 의 위치 계산
	panelSlot = Cast<UCanvasPanelSlot>(Slot);
	SetPostion();

	// 이미지 갱신
	thumbnail->SetBrushFromTexture(itemData.thumbnail);

	// onHover 에 InObject 에 존재하는 함수 InFunctionName 을 등록
	onHover.BindUFunction(InObject, InFunctionName);
}

void UInvenItem::BeginMove()
{
	isMove = true;

	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	offsetMouse = mousePos - panelSlot->GetPosition();
	// 최상위로 보이게 zorder 값을 높여주자
	panelSlot->SetZOrder(100);
}

void UInvenItem::EndMove()
{
	isMove = false;

	panelSlot->SetZOrder(0);


	// 마우스 좌표가 Inven 의 어느 Index 에 있는 계산
	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	int32 m = mousePos.X /* - (인벤상단X) */ / 100;
	int32 n = mousePos.Y /* - (인벤상단Y) */ / 100;
	// Inven 안에 존재
	if (m < 5 && n < 5)
	{
		
	}
	// Inven 밖에 존재
	else
	{
		SetPostion();
	}

}

void UInvenItem::SetPostion()
{
	// 인벤토리이에서 item 의 위치 계산
	FVector2D pos;
	pos.X = (idxInInven % 5) * 100;
	pos.Y = (idxInInven / 5) * 100;
	panelSlot->SetPosition(pos);
}

