#include "FGVisionSensingComponent.h"
#include "FGVisionSensingSettings.h"
#include "FGVisionSensingTargetComponent.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

UFGVisionSensingComponent::UFGVisionSensingComponent()
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UFGVisionSensingComponent::BeginPlay()
{
	Super::BeginPlay();
	TraceDelegate.BindUObject(this, &UFGVisionSensingComponent::OnTraceCompleted);
}

void UFGVisionSensingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SetWantsTrace();
	
	if (LastTraceHandle._Data.FrameNumber != 0)
	{
		FTraceDatum OutData;
		if (GetWorld()->QueryTraceData(LastTraceHandle, OutData))
		{
			// Clear out handle so next tick we don't enter
			LastTraceHandle._Data.FrameNumber = 0;
			// trace is finished, do stuff with results
			DoWorkWithTraceResults(OutData);
		}
	}

	
	
	
	if (SensingSettings == nullptr)
		return;

	const FVector Direction = GetOwner()->GetActorForwardVector();
	const FVector Origin = GetOwner()->GetActorLocation();

	if (bDebugDrawVision)
	{
		FVector Right = Direction.RotateAngleAxis(SensingSettings->Angle, FVector::UpVector);
		FVector Left = Direction.RotateAngleAxis(-SensingSettings->Angle, FVector::UpVector);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Right * SensingSettings->DistanceMinimum, FLinearColor::Red);
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Origin, Origin + Left * SensingSettings->DistanceMinimum, FLinearColor::Green);
	}

	for (int32 Index = SensedTargets.Num() - 1; Index >= 0; --Index)
	{
		UFGVisionSensingTargetComponent* Target = SensedTargets[Index];

		if (Target == nullptr || (Target != nullptr && Target->IsBeingDestroyed()))
		{
			SensedTargets.RemoveAt(Index);
			continue;
		}

		if (!IsPointVisible(Target->GetTargetOrigin(), Origin, Direction, SensingSettings->DistanceMinimum))
		{
			FFGVisionSensingResults Results;
			Results.SensedActor = Target->GetOwner();
			OnTargetLost.Broadcast(Results);
			SensedTargets.RemoveAt(Index);
			CurrentTarget = nullptr;
		}
	}

	TArray<UFGVisionSensingTargetComponent*> ListOfTargets;
	
	UFGVisionSensingTargetComponent::GetSensingTargets(ListOfTargets, GetOwner()->GetActorLocation(), SensingSettings->DistanceMinimum);
	if (bWantsTrace)
	{
		for (UFGVisionSensingTargetComponent* Target : ListOfTargets)
		{
			if (!SensedTargets.Contains(Target))
			{
				if(IsPointVisible(Target->GetTargetOrigin(), Origin, Direction, SensingSettings->DistanceMinimum))
				{
					CurrentTarget = Target;
					SensedTargets.Add(Target);
					FFGVisionSensingResults Results;
					Results.SensedActor = Target->GetOwner();
				}
				
			}
		}
		if (CurrentTarget != nullptr)
			LastTraceHandle = RequestTrace(GetOwner()->GetActorLocation(), CurrentTarget->GetTargetOrigin());
		bWantsTrace = false;
	}
}

bool UFGVisionSensingComponent::IsPointVisible(const FVector& PointToTest, const FVector& Origin, const FVector& Direction, float DistanceMinimum) const
{
	if (SensingSettings == nullptr)
		return false;

	float DistanceMinimumSq = FMath::Square(DistanceMinimum);

	if (FVector::DistSquared(Origin, PointToTest) > DistanceMinimumSq)
	{
		return false;
	}

	const FVector DirectionToTarget = (PointToTest - Origin).GetSafeNormal();

	const float AsHalfRad = FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
	const float Dot = FVector::DotProduct(Direction, DirectionToTarget);

	const bool bIsValid = Dot > AsHalfRad;
	
	return bIsValid;
}

float UFGVisionSensingComponent::GetVisionInRadians() const
{
	if (SensingSettings == nullptr)
		return 0.0;

	return FMath::Cos(FMath::DegreesToRadians(SensingSettings->Angle));
}

void UFGVisionSensingComponent::SetWantsTrace()
{
	// don't allow overlapping traces here.
	if (!GetWorld()->IsTraceHandleValid(LastTraceHandle,false))
	{
		bWantsTrace = true;
	}
}

FTraceHandle UFGVisionSensingComponent::RequestTrace(FVector Start, FVector End)
{
	UWorld* World = GetWorld();
	if (World == nullptr)
		return FTraceHandle();

	auto Channel = UEngineTypes::ConvertToCollisionChannel(MyTraceType);
	
	// Create a FCollisionQueryParams and send it instead of the default below
	// auto Params = UKismetSystemLibrary::ConfigureCollisionParams(NAME_AsyncRequestTrace, bTraceComplex, ActorsToIgnore, bIgnoreSelf, this);
	// const FCollisionObjectQueryParams ObjectQueryParams(Channel);
	//
	// bool bTraceComplex = false;
	// bool bIgnoreSelf = true;
	// TArray<AActor*> ActorsToIgnore;
	
	AActor* ActorToIgnore = GetOwner();

	FCollisionQueryParams Params = FCollisionQueryParams("NAME_AsyncRequestTrace",false, ActorToIgnore);
	return World->AsyncLineTraceByChannel(EAsyncTraceType::Single, 
        Start, End, 
        Channel, 
        Params,
        FCollisionResponseParams::DefaultResponseParam, 
        &TraceDelegate);
	
}

void UFGVisionSensingComponent::OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	ensure(Handle == LastTraceHandle);
	DoWorkWithTraceResults(Data);
	LastTraceHandle._Data.FrameNumber = 0; // reset it
}

void UFGVisionSensingComponent::DoWorkWithTraceResults(const FTraceDatum& TraceData)
{
	// do things here
	if (TraceData.OutHits.Num() <= 0) return;
	
	FFGVisionSensingResults Results;
	AActor* Actor = TraceData.OutHits[0].GetActor();
	Results.SensedActor = Actor;
	OnTargetSensed.Broadcast(Results);
}
