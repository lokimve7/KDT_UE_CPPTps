// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimPlayer.h"
#include "TpsPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UAnimPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 내가 붙어있는 Pawn 을 가져오자
	ATpsPlayer* pawn = Cast<ATpsPlayer>(TryGetPawnOwner());
	if (pawn != nullptr)
	{
		// 나의 앞방향
		FVector forward = pawn->GetActorForwardVector();
		// 이동 방향
		FVector moveDir = pawn->GetVelocity();
		// forward 와 moveDir 의 내적 = cos(두 벡터가 이루는 각) 
		// 위 값이 0보다 크면 앞 방향, 0보다 작으면 뒷 방향
		dirF = FVector::DotProduct(forward, moveDir);
		//dirF = forward.Dot(moveDir);

		//// 각도로 표현해보자 ( 각도 = arcos (두 벡터의 내적)) 
		//// 단 각도는 radian 로 표현된다.
		//float dot = FVector::DotProduct(forward, moveDir.GetSafeNormal());
		//float radianAngle = FMath::Acos(dot);
		//float degreeAngle = radianAngle * (180 / UE_PI);
		////FMath::DegreesToRadians(90);
		////FMath::RadiansToDegrees(radianAngle)
		//UE_LOG(LogTemp, Warning, TEXT("angle : %f"), degreeAngle);
		

		// 나의 오른쪽 방향
		FVector right = pawn->GetActorRightVector();
		// right 와 moveDir 의 내적 = cos(두 벡터가 이루는 각) 
		// 위 값이 0보다 크면 오른쪽 방향, 0보다 작으면 왼쪽 방향
		dirR = FVector::DotProduct(right, moveDir);
		//UE_LOG(LogTemp, Warning, TEXT("dot : %f"), dirR);
		
		// 플레이어가 공중에 있는지 여부
		UCharacterMovementComponent* movement = pawn->GetCharacterMovement();
		isAir = movement->IsFalling();
	}
}
