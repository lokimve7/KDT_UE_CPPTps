// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* hpBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* hpText;

public:
	void UpdateHP(float curHP, float maxHP);
};
