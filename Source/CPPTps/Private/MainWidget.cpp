// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include <Components/TextBlock.h>
#include <Components/ProgressBar.h>

void UMainWidget::UpdateHP(float curHP, float maxHP)
{
	// hpText ���� (50 / 100)
	FString hp = FString::Printf(TEXT("%.0f / %.0f"), curHP, maxHP);
	hpText->SetText(FText::FromString(hp));

	// hpBar ����
	float percent = curHP / maxHP;
	hpBar->SetPercent(percent);
}
