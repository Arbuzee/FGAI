// Fill out your copyright notice in the Description page of Project Settings.

#include "FGHearingSensingComponent.h"

#include "FGAI/FGGameInstance.h"
#include "FGNoiseActor.h"
#include "FGNoiseComponent.h"
#include "DSP/Osc.h"

UFGHearingSensingComponent::UFGHearingSensingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFGHearingSensingComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFGHearingSensingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Distracted) return;
	
	TArray<AFGNoiseActor*> NoiseActors;
	NoiseActors = Cast<UFGGameInstance>(GetWorld()->GetGameInstance())->GetNoiseActors();

	FVector MyLocation = GetOwner()->GetActorLocation();
	float MyRadiusSq = FMath::Square(HearingRange);
	
	for (AFGNoiseActor* NoiseActor : NoiseActors)
	{
		float DistanceToNoiseSq = FVector::DistSquared(NoiseActor->GetActorLocation(), MyLocation);
		float NoiseRangeSq = FMath::Square(NoiseActor->GetInfo().Range);

		if (DistanceToNoiseSq < (NoiseRangeSq + MyRadiusSq))
		{
			FFGHearingInfo HearingInfo;
			HearingInfo.NoiseInstigator = NoiseActor->GetInfo().NoiseInstigator;
			HearingInfo.Range = FMath::Sqrt(DistanceToNoiseSq);
			Distracted = true;
			OnNoiseHeard.Broadcast(HearingInfo);
		}
	}
}

