// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenWidget.h"
#include "Inventory.h"
#include "InvenItem.h"
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>

void UInvenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	itemFactory = LoadClass<UInvenItem>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Inventory/BP_InvenItem.BP_InvenItem_C'"));
}

void UInvenWidget::RefreshInven(TArray<FItemData> myItems)
{
	// myItems 갯수만큼 InvenItem 생성
	for (int i = 0; i < myItems.Num(); i++)
	{
		// InvenItem 생성
		UInvenItem* item = CreateWidget<UInvenItem>(GetWorld(), itemFactory);

		// ** invenPanel 붙이고 나서 위치값을 설정하자!! **
		
		// 인벤토리에 생성한 item 을 붙이자
		invenPanel->AddChild(item);

		// 인벤토리이에서 item 의 위치 계산
		FVector2D pos;
		pos.X = (i % 5) * 100;
		pos.Y = (i / 5) * 100;
		UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(item->Slot);
		panelSlot->SetPosition(pos);	

		// 이미지 갱신
		item->thumbnail->SetBrushFromTexture(myItems[i].thumbnail);
	}
}
