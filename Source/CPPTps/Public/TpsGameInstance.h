// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TpsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPPTPS_API UTpsGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	
public:
	// 가상의 아이템 정보 정의
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<struct FItemData> defineItem;


};
