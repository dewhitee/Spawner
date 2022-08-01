// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerObject.h"

#include "Spawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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
			if (/*CurrentCount*/GetSpawnedCount(Entry.ClassToSpawn, CurrentIndex) < Entry.Count.Get())
			{
				FSpawnArgs SpawnArgs;
				SpawnArgs.ClassToSpawn = Entry.ClassToSpawn;
				
				bool bShouldSkip = false;
				SpawnArgs.AtLocation = GetSpawnLocation(Args, bShouldSkip);
				if (Args.bSkipIfStillNotOnSurface && bShouldSkip)
				{
					UE_LOG(LogSpawner, Warning, TEXT("%s: Spawn of %s is skipped because surface not found under target location."),
						*GetName(), *Entry.ClassToSpawn->GetName());
					return;
				}
				
				SpawnArgs.CollisionHandlingMethod = Args.CollisionHandlingMethod;

				switch (Args.SpawnMode)
				{
				case ESpawnMode::None:
					break;
					
				case ESpawnMode::SpawnAlways:
					Execute_Spawn(this, SpawnArgs);
					break;
					
				case ESpawnMode::SpawnWhenPlayerIsNear:
				{
					const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
					check(PlayerPawn != nullptr);
					const float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), SpawnArgs.AtLocation);
					if (Distance < Args.SpawnEnabledRadius)
					{
						Execute_Spawn(this, SpawnArgs);
					}
					break;
				}	
					
				default: ;
				}
			}
			else // Spawning next actor in SpawnList
			{
				FTimerManager& TimerManager = GetWorld()->GetTimerManager();
				TimerManager.ClearTimer(SpawnTimerHandle);
				CurrentCount_DEPRECATED = 0;
				if (SpawnList.IsValidIndex(++CurrentIndex))
				{
					TimerManager.SetTimer(SpawnTimerHandle, Delegate, Entry.Time.Get(), true);
				}
				else if (Args.bRespawnAfter) // Respawning from the start
				{
					CurrentIndex = 0;
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
		CurrentCount_DEPRECATED++;
		OnSpawn.Broadcast(SpawnedActor, Args);
#if WITH_EDITOR
		UE_LOG(LogSpawner, Log, TEXT("%s: %s was spawned at %s location"), *GetName(), *SpawnedActor->GetActorLabel(),
			*Args.AtLocation.ToCompactString());
#endif

		AddNewSpawnedActor(SpawnedActor, CurrentIndex);
		SpawnedActor->OnDestroyed.AddDynamic(this, &USpawnerObject::OnSpawnedActorDestroyed);
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

FVector USpawnerObject::GetSpawnLocation(const FSpawnStartArgs& Args, bool& bShouldSkip) const
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
		FVector OutLocation = FVector(Args.AtLocation.X + NewPointInRadius.X, Args.AtLocation.Y + NewPointInRadius.Y, Args.AtLocation.Z);

		if (Args.bSnapToSurface)
		{
			SnapToSurface(OutLocation, bShouldSkip, Args);
		}
		
		return OutLocation;
	}
	else
	{
		UE_LOG(LogSpawner, Verbose, TEXT("%s: Args.bUseOuterLocation=false"), *GetName());
	}
	return Args.AtLocation;
}

void USpawnerObject::SnapToSurface(FVector& OutLocation, bool& bShouldSkip, const FSpawnStartArgs& Args) const
{
	FHitResult DownHit;
	const FVector DownEnd = OutLocation - FVector(0.f, 0.f, Args.Radius);
	GetWorld()->LineTraceSingleByChannel(DownHit, OutLocation, DownEnd, ECollisionChannel::ECC_Visibility);
	DrawDebugLineTrace(Args, DownHit, OutLocation, DownEnd, FColor::Blue);

	FHitResult UpHit;
	const FVector UpEnd	= OutLocation + FVector(0.f, 0.f, Args.Radius);
	GetWorld()->LineTraceSingleByChannel(UpHit, OutLocation, UpEnd, ECollisionChannel::ECC_Visibility);
	DrawDebugLineTrace(Args, UpHit, OutLocation, UpEnd, FColor::Yellow);
	
	if (UpHit.bBlockingHit)
	{
		// Also, in case we hit a non-landscape object, we should trace from the top to bottom to find walkable surface on-top of that object
		FHitResult UpToBottomHit;
		const FVector Start = OutLocation + FVector(0.f, 0.f, Args.Radius);
		const FVector End = UpHit.Location - FVector(0.f, 0.f, Args.Radius - UpHit.Location.Z);
		GetWorld()->LineTraceSingleByChannel(UpToBottomHit, Start, End, ECollisionChannel::ECC_Visibility);
		DrawDebugLineTrace(Args, UpToBottomHit, Start, End, FColor::Red);
		
		if (UpToBottomHit.bBlockingHit)
		{
			// We will use UpToBottomHit result structure instead of UpHit in case their location is not the same
			if (!UpHit.Location.Equals(UpToBottomHit.Location, 0.1f))
			{
				UpHit = UpToBottomHit;
			}
		}
	}

	// Find which surface is closer
	UE_LOG(LogSpawner, Verbose, TEXT("%s: UpHit.Distance=%.2f, DownHit.Distance=%.2f"), *GetName(), UpHit.Distance, DownHit.Distance);
	if (!UpHit.bBlockingHit && DownHit.bBlockingHit)
	{
		OutLocation.Z = DownHit.Location.Z;
	}
	else if (UpHit.bBlockingHit && !DownHit.bBlockingHit)
	{
		OutLocation.Z = UpHit.Location.Z;
	}
	else if (UpHit.bBlockingHit && DownHit.bBlockingHit)
	{
		if (UpHit.Distance > DownHit.Distance) // Down is closer
		{
			UE_LOG(LogSpawner, Verbose, TEXT("%s: Down is closer (Z=%.2f)."), *GetName(), DownHit.Location.Z);
			OutLocation.Z = DownHit.Location.Z;
		}
		else // Up is closer
		{
			UE_LOG(LogSpawner, Verbose, TEXT("%s: Up is closer (Z=%.2f)."), *GetName(), UpHit.Location.Z);
			OutLocation.Z = UpHit.Location.Z;
		}
	}
	else
	{
		bShouldSkip = true;
	}
}

void USpawnerObject::DrawDebugLineTrace(const FSpawnStartArgs& Args, const FHitResult& Hit, const FVector& LineTraceStart, const FVector& LineTraceEnd,
	FColor Color, float Lifetime, float Thickness, float HitPointSize) const
{
	if (Args.bDrawDebug)
	{
		DrawDebugLine(GetWorld(), LineTraceStart, LineTraceEnd, Color, false, Lifetime, 0, Thickness);
		if (Hit.bBlockingHit)
		{
			if (Args.bDrawDebug)
			{
				DrawDebugPoint(GetWorld(), Hit.Location, HitPointSize, Color, false, Lifetime);
			}
		}
	}
}

void USpawnerObject::AddNewSpawnedActor(AActor* SpawnedActor, int32 Index)
{
	check(SpawnedActor);
	for (FSpawnedListEntry& Entry : SpawnedActors)
	{
		if (Entry.Index == Index)
		{
			Entry.SpawnedActors.AddUnique(SpawnedActor);
			return;
		}
	}

	SpawnedActors.Add(FSpawnedListEntry(Index, SpawnedActor));
}

void USpawnerObject::OnSpawnedActorDestroyed(AActor* SpawnedActor)
{
	if (SpawnedActor && SpawnedActor->IsPendingKillPending())
	{
		for (FSpawnedListEntry& Entry : SpawnedActors)
		{
			if (Entry.SpawnedActors.Contains(SpawnedActor))
			{
				Entry.SpawnedActors.RemoveSwap(SpawnedActor);
			}
		}
		/*if (FSpawnedListEntry* Entry = SpawnedActors.Find(SpawnedActor->GetClass()))
		{
			Entry->SpawnedActors.Remove(SpawnedActor);
		}*/
	}
}

int32 USpawnerObject::GetSpawnedCount(const TSubclassOf<AActor>& Spawned, int32 Index) const
{
	for (const FSpawnedListEntry& Entry : SpawnedActors)
	{
		if (Entry.Index == Index && Entry.GetClass() == Spawned)
		{
			return Entry.SpawnedActors.Num();
		}
	}
	return 0;
}
