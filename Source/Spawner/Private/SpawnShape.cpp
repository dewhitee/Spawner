// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnShape.h"

#include "ParameterizedSpawnerObject.h"
#include "Spawner.h"
#include "SpawnerObject.h"
#include "DistributionFunctionLibrary.h"

URingSpawnShape::URingSpawnShape()
{
	UE_LOG(LogSpawner, VeryVerbose, TEXT("%s: CONSTRUCTED, Outer=%s"), *GetName(), GetOuter() ? *GetOuter()->GetName() : TEXT("None"));
	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject))
	{
		return;
	}

	UParameterizedSpawnerObject* Spawner = GetSpawner();
	if (!Spawner)
	{
		return;
	}

	const FSpawnStartArgs& SpawnSettings = Spawner->GetSpawnSettings();
	OuterRadius = SpawnSettings.Radius;
	InnerRadius = SpawnSettings.Radius * 0.8f;
}

#if WITH_EDITOR
void URingSpawnShape::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property)
	{
		const FName PropertyName = PropertyChangedEvent.GetPropertyName();
		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSpawnShape, OuterRadius))
		{
			if (OuterRadius < InnerRadius)
			{
				OuterRadius = InnerRadius;
			}
		}

		if (PropertyName == GET_MEMBER_NAME_CHECKED(URingSpawnShape, InnerRadius))
		{
			if (InnerRadius > OuterRadius)
			{
				InnerRadius = OuterRadius;
			}
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

FVector URingSpawnShape::GetLocationInShape() const
{
	check(GetSpawner());
	FDistributionRingArgs DistributionArgs;
	DistributionArgs.Count = 1;
	DistributionArgs.OuterRadius = OuterRadius;
	DistributionArgs.InnerRadius = InnerRadius;
	if (const AActor* OuterActor = GetTypedOuter<AActor>())
	{
		DistributionArgs.Location = OuterActor->GetActorLocation();
	}
	FDistributionData Data = UDistributionFunctionLibrary::DistributeRing(DistributionArgs);
	return Data.Points[0];
}

UParameterizedSpawnerObject* URingSpawnShape::GetSpawner() const
{
	return Cast<UParameterizedSpawnerObject>(GetOuter());;
}
