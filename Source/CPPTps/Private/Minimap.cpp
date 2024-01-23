// Fill out your copyright notice in the Description page of Project Settings.


#include "Minimap.h"
#include <Components/SceneCaptureComponent2D.h>
#include <Engine/TextureRenderTarget2D.h>
#include <Kismet/GameplayStatics.h>
#include "TpsPlayer.h"

// Sets default values
AMinimap::AMinimap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// USceneCaptureComponent2D 추가
	compCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CAPTURE"));
	SetRootComponent(compCapture);

	
	// Render Target 읽어와서 셋팅
	ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> tempTex(TEXT("/Script/Engine.TextureRenderTarget2D'/Game/Resources/RT_Mimap.RT_Mimap'"));
	if (tempTex.Succeeded())
	{
		compCapture->TextureTarget = tempTex.Object;
	}

}

// Called when the game starts or when spawned
void AMinimap::BeginPlay()
{
	Super::BeginPlay();
	
	// 플레이어 찾자
	player = UGameplayStatics::GetActorOfClass(GetWorld(), ATpsPlayer::StaticClass());

}

// Called every frame
void AMinimap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어의 x, y 좌표만 나에게 셋팅
	FVector myPos = GetActorLocation();
	FVector playerPos = player->GetActorLocation();
	myPos.X = playerPos.X;
	myPos.Y = playerPos.Y;
	SetActorLocation(myPos);
}

