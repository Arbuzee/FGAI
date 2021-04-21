// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AI/Sensing/FGNoiseActor.h"
#include "Engine/GameInstance.h"
#include "FGGameInstance.generated.h"

class AFGNoiseActor;

UCLASS()
class FGAI_API UFGGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void AddNoiseActor(AFGNoiseActor* NoiseActor);
	virtual void RemoveNoiseActor(AFGNoiseActor* NoiseActor);
	TArray<AFGNoiseActor*> GetNoiseActors() { return NoiseActors; }
	
protected:
	UPROPERTY()
	TArray<AFGNoiseActor*> NoiseActors;
};
