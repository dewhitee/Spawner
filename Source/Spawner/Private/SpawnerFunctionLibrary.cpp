// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnerFunctionLibrary.h"

#include "ParameterizedSpawnerObject.h"
#include "Spawner.h"
#include "SpawnShape.h"

void USpawnerFunctionLibrary::PushToSpawn(UParameterizedSpawnerObject* Spawner, AActor* ActorToPush, float DistanceThreshold)
{
	if (!Spawner || !ActorToPush)
	{
		return;
	}

	if (const AActor* SpawnerOwner = Spawner->GetTypedOuter<AActor>())
	{
		const float Distance = FVector::Dist(SpawnerOwner->GetActorLocation(), ActorToPush->GetActorLocation());
		if (Distance < DistanceThreshold)
		{
			return;
		}
	}

	if (const USpawnShape* Shape = Spawner->GetSpawnSettings().SpawnShapeSettings.SpawnShape)
	{
		FVector NewLoc = Shape->GetLocationInShape();
		bool bShouldSkip = false;
		
		Spawner->SnapToSurfaceParameterized(NewLoc, bShouldSkip);
		
		UE_LOG(LogSpawner, Verbose, TEXT("USpawnerFunctionLibrary::PushToSpawn: Pushed %s location from %s to %s"),
			*ActorToPush->GetActorNameOrLabel(),
			*ActorToPush->GetActorLocation().ToCompactString(),
			*NewLoc.ToCompactString());

		if (!bShouldSkip)
		{
			ActorToPush->SetActorLocation(NewLoc);
		}
	}
}
