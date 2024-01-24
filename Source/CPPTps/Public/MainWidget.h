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



	// �ٲ�� ���� HP
	float startHP = 0;
	// �ٲ��� �ϴ� HP
	float endHP = 0;
	// �ִ� HP
	float maxHP;
	// ����
	float ratioHP = 10;

	// ���� ��� �ִ� ���� type
	int32 weaponType;
	// �� UI ������ �ð�
	float weaponTime;

public:
	void InitHP(float mHP);
	void UpdateHP(float cHP);
	void UpdateWeapon(int32 type);
	void SetWeaponZOrder(int32 rifleZ, int32 sniperZ);

	void ShowWeapon();
};
