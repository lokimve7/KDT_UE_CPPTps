// Fill out your copyright notice in the Description page of Project Settings.


#include "InvenWidget.h"
#include "Inventory.h"
#include "InvenItem.h"
#include <Components/CanvasPanel.h>
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

	// 만들어진 item widget 을 array 가지고 있자
	allItemWidget.Add(item);

	// ** invenPanel 붙이고 나서 위치값을 설정하자!! **

	// 인벤토리에 생성한 item 을 붙이자
	invenPanel->AddChild(item);	

	// itemData 를 만들어진 item 에 알려주자
	item->InitItem(itemData, i, player, TEXT("OnHoverItem"));
}

void UInvenWidget::SwitchItem(int32 dest, int32 sour)
{
	// dest, sour 에 있는 idxInInven 값을 바꾸자
	allItemWidget[dest]->idxInInven = sour;
	allItemWidget[sour]->idxInInven = dest;

	// allItemWidget 에 들어있는 순서도 바꾸자
	UInvenItem* temp = allItemWidget[sour];
	allItemWidget[sour] = allItemWidget[dest];
	allItemWidget[dest] = temp;

	// 서로 바꾼 위치로 다시 갱신하자
	allItemWidget[dest]->SetPostion();
	allItemWidget[sour]->SetPostion();
}
