// Fill out your copyright notice in the Description page of Project Settings.

#include "FGNoiseActor.h"
#include "DrawDebugHelpers.h"
#include "FGHearingSensingComponent.h"
#include "FGAI/FGGameInstance.h"

AFGNoiseActor::AFGNoiseActor()
{
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	OnDestroyed.AddDynamic(this, &AFGNoiseActor::WhenDestroyed);
}

void AFGNoiseActor::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(1.f);
}

void AFGNoiseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DrawDebugSphere(GetWorld(), GetActorLocation(), _Radius, 10, FColor::Silver,
		false, 0.2f, 0, 0);
}

void AFGNoiseActor::SetupInfo(AActor* NoiseInstigator, float Range)
{
	_NoiseInstigator = NoiseInstigator;
	_Radius = Range;
}

FFGHearingInfo AFGNoiseActor::GetInfo()
{
	FFGHearingInfo Info;
	Info.NoiseInstigator = _NoiseInstigator;
	Info.Range = _Radius;
	
	return Info;
}

void AFGNoiseActor::WhenDestroyed(AActor* Act)
{
	Cast<UFGGameInstance>(GetWorld()->GetGameInstance())->RemoveNoiseActor(this);
}



