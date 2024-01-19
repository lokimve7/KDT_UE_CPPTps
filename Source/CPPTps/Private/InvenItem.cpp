// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenItem.h"
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>
#include <Blueprint/WidgetLayoutLibrary.h>

// ���콺 (����/������) ������ (���콺�����Ͱ� �� ���� ���� ��)
FReply UInvenItem::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	onHover.ExecuteIfBound(this);

	//UE_LOG(LogTemp, Warning, TEXT("item name : %s"), *itemData.name);

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UInvenItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// ���콺�� ���� ������ �� �ִٸ�
	if (isMove)
	{
		// ���콺 ��ġ�� ���� ��ġ�� ����
		FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		panelSlot->SetPosition(mousePos - offsetMouse);
	}
}

void UInvenItem::InitItem(FItemData data, int32 idx, UObject* InObject, const FName& InFunctionName)
{
	itemData = data;
	idxInInven = idx;

	// �κ��丮�̿��� item �� ��ġ ���
	panelSlot = Cast<UCanvasPanelSlot>(Slot);
	SetPostion();

	// �̹��� ����
	thumbnail->SetBrushFromTexture(itemData.thumbnail);

	// onHover �� InObject �� �����ϴ� �Լ� InFunctionName �� ���
	onHover.BindUFunction(InObject, InFunctionName);
}

void UInvenItem::BeginMove()
{
	isMove = true;

	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	offsetMouse = mousePos - panelSlot->GetPosition();
	// �ֻ����� ���̰� zorder ���� ��������
	panelSlot->SetZOrder(100);
}

int32 UInvenItem::EndMove()
{
	isMove = false;

	panelSlot->SetZOrder(0);


	// ���콺 ��ǥ�� Inven �� ��� Index �� �ִ� ���
	FVector2D mousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	int32 m = mousePos.X /* - (�κ����X) */ / 100;
	int32 n = mousePos.Y /* - (�κ����Y) */ / 100;
	// Inven �ȿ� ����
	if (m < 5 && n < 5)
	{
		return m + n * 5;
	}
	// Inven �ۿ� ����
	else
	{
		return -1;
		//SetPostion();
	}
}

void UInvenItem::SetPostion()
{
	// �κ��丮�̿��� item �� ��ġ ���
	FVector2D pos;
	pos.X = (idxInInven % 5) * 100;
	pos.Y = (idxInInven / 5) * 100;
	panelSlot->SetPosition(pos);
}

