// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>
#include "TpsPlayer.h"

void UMainWidget::UpdateHP(float curHP, float maxHP)
{
	// hpText 갱신 (50 / 100)
	FString hp = FString::Printf(TEXT("%.0f / %.0f"), curHP, maxHP);
	hpText->SetText(FText::FromString(hp));

	// hpBar 갱신
	float percent = curHP / maxHP;
	hpBar->SetPercent(percent);
}

void UMainWidget::UpdateWeapon(int32 type)
{
	EWeaponType e = (EWeaponType)type;
	if (e == EWeaponType::GUN)
	{
		SetWeaponZOrder(2, 0);
	}

	else if (e == EWeaponType::SNIPER)
	{
		SetWeaponZOrder(0, 2);
	}
}

void UMainWidget::SetWeaponZOrder(int32 rifleZ, int32 sniperZ)
{
	// 라이플 총 UI 
	UCanvasPanelSlot* panelSlot = Cast<UCanvasPanelSlot>(rifle->Slot);
	panelSlot->SetZOrder(rifleZ);

	// 스나이퍼 총 UI
	panelSlot = Cast<UCanvasPanelSlot>(sniper->Slot);
	panelSlot->SetZOrder(sniperZ);
}
