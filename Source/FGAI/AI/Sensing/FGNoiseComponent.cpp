// Fill out your copyright notice in the Description page of Project Settings.

#include "FGNoiseComponent.h"
#include "FGAI/FGGameInstance.h"
#include "FGNoiseActor.h"

UFGNoiseComponent::UFGNoiseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UFGNoiseComponent::SetNoiseRadius(float Radius)
{
	_Radius = Radius;
}

void UFGNoiseComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UFGNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Timer += DeltaTime;
	if (Timer >= FireRate)
	{
		SpawnNoise();
		FireRate++;
	}
}

void UFGNoiseComponent::SpawnNoise()
{
	AFGNoiseActor* NoiseActor = GetWorld()->SpawnActor<AFGNoiseActor>(GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation());
	
	NoiseActor->SetupInfo(GetOwner(), 1000.f);
	Cast<UFGGameInstance>(GetWorld()->GetGameInstance())->AddNoiseActor(NoiseActor);
	
	// UE_LOG(LogTemp, Log, TEXT("Noise Spawned") );
}


