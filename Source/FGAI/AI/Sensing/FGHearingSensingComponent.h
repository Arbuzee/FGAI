// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FGHearingSensingComponent.generated.h"

USTRUCT(BlueprintType)
struct FFGHearingInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AActor* NoiseInstigator = nullptr;

	UPROPERTY(BlueprintReadOnly)
	float Range = 1000.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFGHearingSenseDelegate, const FFGHearingInfo&, Results);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FGAI_API UFGHearingSensingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFGHearingSensingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FFGHearingSenseDelegate OnNoiseHeard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HearingRange = 1000.f;

	UPROPERTY(BlueprintReadWrite)
	bool Distracted = false;
	
protected:
	virtual void BeginPlay() override;
	
};

