#pragma once

#include "Components/ActorComponent.h"
#include "FGVisionSensingComponent.generated.h"

class AActor;
class UFGVisionSensingTargetComponent;
class UFGVisionSensingSettings;

USTRUCT(BlueprintType)
struct FFGVisionSensingResults
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	AActor* SensedActor = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFGVisionSensingDelegate, const FFGVisionSensingResults&, Results);
// DECLARE_DELEGATE_TwoParams( FTraceDelegate, const FTraceHandle&, FTraceDatum &);

UCLASS(Blueprintable, BlueprintType, meta = (BlueprintSpawnableComponent))
class UFGVisionSensingComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UFGVisionSensingComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Direction, float DistanceMinimum) const;

	UPROPERTY(Transient)
	TArray<UFGVisionSensingTargetComponent*> SensedTargets;

	float GetVisionInRadians() const;

	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnTargetSensed;

	UPROPERTY(BlueprintAssignable)
	FFGVisionSensingDelegate OnTargetLost;

	UPROPERTY(EditAnywhere, Category = Sensing)
	UFGVisionSensingSettings* SensingSettings = nullptr;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDebugDrawVision = false;

protected:
	UFGVisionSensingTargetComponent* CurrentTarget;
	
	// BP exposed function to say you would like a trace to occur
	UFUNCTION(BlueprintCallable)
	void SetWantsTrace();
	// BP exposed event that the actor can implement to deal with results in BP
	UFUNCTION(BlueprintImplementableEvent)
	void ReceiveOnTraceCompleted(const TArray<FHitResult> & Results);

	FTraceHandle RequestTrace(FVector Start, FVector End); // Actually do trace request
	void OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data); // Delegate function
	void DoWorkWithTraceResults(const FTraceDatum& TraceData); // Do any real work we want

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETraceTypeQuery> MyTraceType;

	FTraceHandle LastTraceHandle;
	FTraceDelegate TraceDelegate; // Delegate fired when trace is completed
	uint32 bWantsTrace : 1; // Did user request trace?
};
