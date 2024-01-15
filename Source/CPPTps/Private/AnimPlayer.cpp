// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimPlayer.h"

void UAnimPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 내가 붙어있는 Pawn 을 가져오자
	APawn* pawn = TryGetPawnOwner();
	if (pawn != nullptr)
	{
		moveSpeed = pawn->GetVelocity().Length();
	}
}
