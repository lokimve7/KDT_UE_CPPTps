// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimEnemy.h"

void UAnimEnemy::AnimNotify_OnDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDamage!!!!!!"));
}
