// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>
#include "TpsPlayer.h"

void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//startHP = FMath::Lerp(startHP, endHP, InDeltaTime * 10);
	//// hpText 갱신 (50 / 100)
	//FString strHp = FString::Printf(TEXT("%.0f / %.0f"), startHP, maxHP);
	//hpText->SetText(FText::FromString(strHp));

	//// hpBar 갱신
	//float percent = startHP / maxHP;
	//hpBar->SetPercent(percent);
	

	if (ratioHP < 1)
	{
		ratioHP += InDeltaTime * 4;
		if(ratioHP > 1) ratioHP = 1;

		float hp = FMath::Lerp(startHP, endHP, ratioHP);

		// hpText 갱신 (50 / 100)
		FString strHp = FString::Printf(TEXT("%.0f / %.0f"), hp, maxHP);
		hpText->SetText(FText::FromString(strHp));

		// hpBar 갱신
		float percent = hp / maxHP;
		hpBar->SetPercent(percent);
	}
}

void UMainWidget::InitHP(float mHP)
{
	startHP = endHP = maxHP = mHP;
}

void UMainWidget::UpdateHP(float cHP)
{
	// 바뀌기 전 HP 설정
	startHP = endHP;
	// 바껴야 하는 HP 설정
	endHP = cHP;	
	// 비율 값을 0
	ratioHP = 0;
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
