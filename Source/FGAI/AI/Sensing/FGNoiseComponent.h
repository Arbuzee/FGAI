// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FGNoiseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FGAI_API UFGNoiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFGNoiseComponent();
	
	UFUNCTION(BlueprintCallable)
	void SetNoiseRadius(float Radius);
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SpawnNoise();

	float _Radius;

	UPROPERTY(EditDefaultsOnly, Category = Noise)
	float FireRate = 0.f;
	float Timer = 0.f;
};
