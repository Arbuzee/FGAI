// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FGNoiseActor.generated.h"

struct FFGHearingInfo;

UCLASS()
class FGAI_API AFGNoiseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFGNoiseActor();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInfo(AActor* NoiseInstigator, float Range);
	virtual FFGHearingInfo GetInfo();
	
	UFUNCTION()
    void WhenDestroyed(AActor* Act);
	
protected:
	virtual void BeginPlay() override;
	
	
	UPROPERTY()
	USceneComponent* SceneComponent;

	float _Radius = 300.f;
	
	UPROPERTY()
	AActor* _NoiseInstigator;
};
