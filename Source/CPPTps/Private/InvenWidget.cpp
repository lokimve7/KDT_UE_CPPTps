// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenWidget.h"
#include "Inventory.h"
#include "InvenItem.h"
#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"

void UInvenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	itemFactory = LoadClass<UInvenItem>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/Inventory/BP_InvenItem.BP_InvenItem_C'"));

	// 플레이어를 찾자
	player = Cast<ATpsPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass()));
}

void UInvenWidget::RefreshInven(TArray<FItemData> myItems)
{
	// 나한테 붙어있는 InvenItem 모두 삭제
	for (int32 i = 0; i < allItemWidget.Num(); i++)
	{
		allItemWidget[i]->RemoveFromParent();
	}
	allItemWidget.Empty();

	// myItems 갯수만큼 InvenItem 생성
	for (int i = 0; i < myItems.Num(); i++)
	{
		CreateItem(myItems[i], i);
	}
}

void UInvenWidget::CreateItem(FItemData itemData, int32 i)
{
	// InvenItem 생성
	UInvenItem* item = CreateWidget<UInvenItem>(GetWorld(), itemFactory);

	// itemData 를 만들어진 item 에 알려주자
	item->InitItem(itemData, i, player, TEXT("OnHoverItem"));

	// 만들어진 item widget 을 array 가지고 있자
	allItemWidget.Add(item);

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
	item->thumbnail->SetBrushFromTexture(itemData.thumbnail);
}
