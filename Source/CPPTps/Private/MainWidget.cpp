// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/Image.h>
#include "TpsPlayer.h"
#include "EasingLibrary.h"

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

	// 만약에 나타나야 하는 무기UI 있다면
	if (weaponType > 0)
	{
		ShowWeapon();
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
	// UI 가 나타나야 하는 무기 종류
	weaponType = type;
	weaponTime = 0;

	EWeaponType e = (EWeaponType)type;
	if (e == EWeaponType::GUN)
	{
		SetWeaponZOrder(2, 0);
				
		// weaponSniper 의 위치를 0, 0 으로 하자
		Cast<UCanvasPanelSlot>(weaponSniper->Slot)->SetPosition(FVector2D::ZeroVector);
	}

	else if (e == EWeaponType::SNIPER)
	{
		SetWeaponZOrder(0, 2);

		// weaponRifle 의 위치를 0, 0 으로 하자
		Cast<UCanvasPanelSlot>(weaponRifle->Slot)->SetPosition(FVector2D::ZeroVector);
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

void UMainWidget::ShowWeapon()
{
	// 시간을 증가
	weaponTime += GetWorld()->GetDeltaSeconds();

	// 목적지까지 다 도착했다면
	if(weaponTime > 1) 
	{
		weaponTime = 1;

		// 나타나야하는 무기 UI 를 없앤다.
		weaponType = 0;
	}

	float ratio = UEasingLibrary::EaseOutBounce(weaponTime);
	FVector2D pos = FMath::Lerp(FVector2D::ZeroVector, FVector2D(-300, 0), ratio);

	if (weaponType == 1)
	{
		Cast<UCanvasPanelSlot>(weaponRifle->Slot)->SetPosition(pos);
	}
	else if(weaponType == 2)
	{
		Cast<UCanvasPanelSlot>(weaponSniper->Slot)->SetPosition(pos);
	}
}
