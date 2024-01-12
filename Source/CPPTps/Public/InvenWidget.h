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
	TSubclassOf<class UInvenItem> itemFactory;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* invenPanel;

public:
	void RefreshInven(TArray<struct FItemData> myItems);
};
