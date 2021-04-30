#pragma once

#include "AI/Sensing/FGNoiseComponent.h"
#include "GameFramework/Pawn.h"
#include "FGCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class UFGMovementComponent;
class UFGVisionSensingTargetComponent;
class UFGNoiseComponent;

UCLASS()
class AFGCharacter : public APawn
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Collision)
	UCapsuleComponent* Capsule;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Movement)
	UFGMovementComponent* MovementComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = VisionTarget)
	UFGVisionSensingTargetComponent* VisionSensingTargetComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Noise)
	UFGNoiseComponent* NoiseComponent;
	
	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	AFGCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float Speed;

	UPROPERTY(EditAnywhere, Category = Movement)
	float Gravity;

	FVector InputVector = FVector::ZeroVector;
	FVector LookVector = FVector::ZeroVector;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void OnFire();
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UCapsuleComponent* GetCapsule() const { return Capsule; }
};

