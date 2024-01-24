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
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* hpBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* hpText;

	UPROPERTY(meta = (BindWidget))
	class UImage* rifle;

	UPROPERTY(meta = (BindWidget))
	class UImage* sniper;

	UPROPERTY(meta = (BindWidget))
	class UImage* weaponRifle;

	UPROPERTY(meta = (BindWidget))
	class UImage* weaponSniper;



	// 바뀌기 전의 HP
	float startHP = 0;
	// 바껴야 하는 HP
	float endHP = 0;
	// 최대 HP
	float maxHP;
	// 비율
	float ratioHP = 10;

	// 현재 들고 있는 총의 type
	int32 weaponType;
	// 총 UI 나오는 시간
	float weaponTime;

public:
	void InitHP(float mHP);
	void UpdateHP(float cHP);
	void UpdateWeapon(int32 type);
	void SetWeaponZOrder(int32 rifleZ, int32 sniperZ);

	void ShowWeapon();
};
