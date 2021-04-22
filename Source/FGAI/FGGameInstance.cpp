// Fill out your copyright notice in the Description page of Project Settings.


#include "FGGameInstance.h"

void UFGGameInstance::AddNoiseActor(AFGNoiseActor* NoiseActor)
{
	NoiseActors.Add(NoiseActor);
	// UE_LOG(LogTemp, Log, TEXT("Noise Spawned %s"), NoiseActors.Num() );
}

void UFGGameInstance::RemoveNoiseActor(AFGNoiseActor* NoiseActor)
{
	NoiseActors.Remove(NoiseActor);
}
