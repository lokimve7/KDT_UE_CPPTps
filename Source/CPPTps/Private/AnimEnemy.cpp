// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimEnemy.h"
#include "Enemy.h"


void UAnimEnemy::AnimNotify_OnDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDamage!!!!!!"));
	
	// 1. ��(Pawn, Character, Enemy) �� ã��
	APawn* pawn = TryGetPawnOwner();
	AEnemy* myPawn = Cast<AEnemy>(pawn);
	// 2. target �� ������ �ִ� DamageProcess �Լ� ����
	myPawn->AttackTarget();
}



