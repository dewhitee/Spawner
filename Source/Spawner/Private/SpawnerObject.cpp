// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerObject.h"

#include "Spawner.h"

UWorld* USpawnerObject::GetWorld() const
{
	if (GIsEditor && !GIsPlayInEditorWorld)
	{
		return nullptr;
	}
	if (GetOuter())
	{
		return GetOuter()->GetWorld();
	}
	return nullptr;
}

void USpawnerObject::Start_Implementation(const FSpawnStartArgs& Args)
{
	OnStart.Broadcast();
	if (bSpawnEnabled)
	{
		Delegate.BindLambda([&, Args]()
		{
			const FSpawnListEntry& Entry = SpawnList[CurrentIndex];
			if (CurrentCount < Entry.Count.Get())
			{
				FSpawnArgs SpawnArgs;
				SpawnArgs.ClassToSpawn = Entry.ClassToSpawn;
				SpawnArgs.AtLocation = GetSpawnLocation(Args);
				SpawnArgs.CollisionHandlingMethod = Args.CollisionHandlingMethod;
				Execute_Spawn(this, SpawnArgs);
			}
			else
			{
				FTimerManager& TimerManager = GetWorld()->GetTimerManager();
				TimerManager.ClearTimer(SpawnTimerHandle);
				CurrentCount = 0;
				if (SpawnList.IsValidIndex(++CurrentIndex))
				{
					TimerManager.SetTimer(SpawnTimerHandle, Delegate, Entry.Time.Get(), true);
				}
			}
		});

		if (SpawnList.IsValidIndex(CurrentIndex))
		{
			const FSpawnListEntry& Entry = SpawnList[CurrentIndex];
			GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, Delegate, Entry.Time.Get(), true);
		}
		else
		{
			// TODO: Log out of bounds
		}
	}
}

void USpawnerObject::Stop_Implementation()
{
	OnStop.Broadcast();
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
}

AActor* USpawnerObject::Spawn_Implementation(const FSpawnArgs& Args)
{
	UE_LOG(LogSpawner, Log, TEXT("%s: Spawning new %s"), *GetName(), *Args.ClassToSpawn->GetName());
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = Args.CollisionHandlingMethod;
	AActor* SpawnedActor = GetWorld()->SpawnActor(Args.ClassToSpawn, &Args.AtLocation, &Args.AtRotation, SpawnParams);
	if (SpawnedActor)
	{
		CurrentCount++;
		OnSpawn.Broadcast(SpawnedActor, Args);
#if WITH_EDITOR
		UE_LOG(LogSpawner, Log, TEXT("%s: %s was spawned at %s location"), *GetName(), *SpawnedActor->GetActorLabel(),
			*Args.AtLocation.ToCompactString());
#endif
	}
	return SpawnedActor;
}

AActor* USpawnerObject::Respawn_Implementation(const FSpawnArgs& Args)
{
	unimplemented();
	return nullptr;
}

void USpawnerObject::SetSpawnEnabled(bool bEnabled)
{
	bSpawnEnabled = bEnabled;
}

void USpawnerObject::SetSpawnList(const TArray<FSpawnListEntry>& Entries)
{
	SpawnList = Entries;
}

void USpawnerObject::SetDelaysList(const TArray<float>& Delays)
{
	DelaysList = Delays;
}

FVector USpawnerObject::GetSpawnLocation(const FSpawnStartArgs& Args) const
{
	if (Args.bUseOuterLocation)
	{
		if (const AActor* OuterActor = GetTypedOuter<AActor>())
		{
			UE_LOG(LogSpawner, Verbose, TEXT("%s: OuterActor->GetActorLocation() = %s"),
				*GetName(), *OuterActor->GetActorLocation().ToCompactString());
			return OuterActor->GetActorLocation();
		}
		UE_LOG(LogSpawner, Warning, TEXT("%s: OuterActor was nullptr"), *GetName());
	}
	else if (Args.bUseRandomLocationInRadius && Args.Radius > 0.f)
	{
		const FVector2D NewPointInRadius = FMath::RandPointInCircle(Args.Radius);
		return FVector(Args.AtLocation.X + NewPointInRadius.X, Args.AtLocation.Y + NewPointInRadius.Y, Args.AtLocation.Z);
	}
	else
	{
		UE_LOG(LogSpawner, Verbose, TEXT("%s: Args.bUseOuterLocation=false"), *GetName());
	}
	return Args.AtLocation;
}
