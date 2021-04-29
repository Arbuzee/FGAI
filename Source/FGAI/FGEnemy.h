#pragma once

#include "GameFramework/Pawn.h"
#include "FGEnemy.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class UFGVisionSensingComponent;
class UFGHearingSensingComponent;
class UFGNavMovementComponent;

UCLASS()
class AFGEnemy : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Collision)
	UCapsuleComponent* Capsule;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Vision)
	UFGVisionSensingComponent* VisionSensingComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Vision)
	UFGHearingSensingComponent* HearingSensingComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Movement)
	UFGNavMovementComponent* NavMovementComponent;

	AFGEnemy();

	virtual float GetDefaultHalfHeight() const override;

protected:
	virtual void BeginPlay();

	UCapsuleComponent* GetCapsule() const { return Capsule; }
};

