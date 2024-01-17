// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenItem.h"

// 마우스 (왼쪽/오른쪽) 눌릴때 (마우스포인터가 내 위에 있을 때)
FReply UInvenItem::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	onHover.ExecuteIfBound(this);

	//UE_LOG(LogTemp, Warning, TEXT("item name : %s"), *itemData.name);

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UInvenItem::InitItem(FItemData data, int32 idx, UObject* InObject, const FName& InFunctionName)
{
	itemData = data;
	idxInInven = idx;

	// onHover 에 InObject 에 존재하는 함수 InFunctionName 을 등록
	onHover.BindUFunction(InObject, InFunctionName);
}


