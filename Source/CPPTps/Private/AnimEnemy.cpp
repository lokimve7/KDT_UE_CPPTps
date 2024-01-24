// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimEnemy.h"
#include "Enemy.h"


void UAnimEnemy::AnimNotify_OnDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("OnDamage!!!!!!"));
	
	// 1. 나(Pawn, Character, Enemy) 를 찾자
	APawn* pawn = TryGetPawnOwner();
	AEnemy* myPawn = Cast<AEnemy>(pawn);
	// 2. target 이 가지고 있는 DamageProcess 함수 실행
	myPawn->AttackTarget();
}



