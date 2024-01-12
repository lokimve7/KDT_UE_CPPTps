// Fill out your copyright notice in the Description page of Project Settings.


#include "TpsGameInstance.h"

#include "Inventory.h"

UTpsGameInstance::UTpsGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempData(TEXT("/Script/Engine.DataTable'/Game/Blueprints/Inventory/ItemTable.ItemTable'"));
	if (tempData.Succeeded())
	{
		UDataTable* data = tempData.Object;

		// 갯수 (행의 갯수)
		TArray<FName> rowNames = data->GetRowNames();
		for (int i = 0; i < rowNames.Num(); i++)
		{
			// 각 행의 데이터를 가져오자
			FItemData* itemData = data->FindRow<FItemData>(rowNames[i], TEXT(""));
			// 우리가 만든 Array 에 담아놓자
			defineItem.Add(*itemData);			
		}
	}
}

void UTpsGameInstance::Init()
{
	Super::Init();

	////LoadClass()
	//// 아이템 정의 - Database 에 있는 값을 읽어서 셋팅)
	//UDataTable* data = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Blueprints/Inventory/ItemTable.ItemTable'"));
	//// 갯수 (행의 갯수)
	//TArray<FName> rowNames = data->GetRowNames();
	//for (int i = 0; i < rowNames.Num(); i++)
	//{
	//	// 각 행의 데이터를 가져오자
	//	FItemData* itemData = data->FindRow<FItemData>(rowNames[i], TEXT(""));
	//	// 우리가 만든 Array 에 담아놓자
	//	defineItem.Add(*itemData);
	//}
}