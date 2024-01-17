// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvenWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UInvenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	class ATpsPlayer* player;

	UPROPERTY()
	TSubclassOf<class UInvenItem> itemFactory;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* invenPanel;

	UPROPERTY()
	TArray<class UInvenItem*> allItemWidget;

public:
	void RefreshInven(TArray<struct FItemData> myItems);
	void CreateItem(FItemData itemData, int32 i);
};
