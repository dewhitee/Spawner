// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerInterface.h"
#include "UObject/NoExportTypes.h"
#include "SpawnerObject.generated.h"

USTRUCT(BlueprintType)
struct FSpawnTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(ClampMin=0))
	float Delay = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(ClampMin=0))
	float RandomTimeScatter = 0.f;

	float Get() const { return Delay + FMath::RandRange(0.f, RandomTimeScatter); }
};

USTRUCT(BlueprintType)
struct FSpawnCount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	int32 Count = 1;
	
	UPROPERTY(EditAnywhere, Category=Spawner, meta=(EditCondition=bRandom))
	FInt32Range RandomCount;

	int32 Get() const
	{
		return bRandom
			? FMath::RandRange(RandomCount.GetLowerBoundValue(), RandomCount.GetUpperBoundValue())
			: Count;
	}
};

USTRUCT(BlueprintType)
struct FSpawnListEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TSubclassOf<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FSpawnTime Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(ClampMin=0))
	FSpawnCount Count;
};

USTRUCT(BlueprintType)
struct FSpawnedListEntry
{
	GENERATED_BODY()

	FSpawnedListEntry() : Index(0) {}
	FSpawnedListEntry(int32 InIndex, AActor* InActor) : Index(InIndex), SpawnedActors({InActor}) {}
	TSubclassOf<AActor> GetClass() const
	{
		return SpawnedActors.IsEmpty() ? nullptr : SpawnedActors[0]->GetClass();
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawned)
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawned)
	TArray<TObjectPtr<AActor>> SpawnedActors;
};

USTRUCT(BlueprintType)
struct FRespawnListEntry
{
	GENERATED_BODY()
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawn, AActor*, SpawnedActor, const FSpawnArgs&, Args);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreSpawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnerStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnerStop);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class SPAWNER_API USpawnerObject : public UObject, public ISpawnerInterface
{
	GENERATED_BODY()

	
	
public:
	virtual UWorld* GetWorld() const override;
	
	// ISpawnerInterface
	virtual void Start_Implementation(const FSpawnStartArgs& Args) override;
	virtual void Stop_Implementation() override;
	virtual AActor* Spawn_Implementation(const FSpawnArgs& Args) override;
	virtual AActor* Respawn_Implementation(const FSpawnArgs& Args) override;

	UFUNCTION(BlueprintCallable, Category=Spawner)
	void SetSpawnEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category=Spawner)
	void SetSpawnList(const TArray<FSpawnListEntry>& Entries);

	UFUNCTION(BlueprintCallable, Category=Spawner)
	void SetDelaysList(const TArray<float>& Delays);
	
	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawn OnSpawn;
	
	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnPreSpawn OnPreSpawn;

	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawnerStop OnStop;
	
	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawnerStart OnStart;
	
private:
	FVector GetSpawnLocation(const FSpawnStartArgs& Args, bool& bShouldSkip) const;
	void SnapToSurface(FVector& OutLocation, bool& bShouldSkip, const FSpawnStartArgs& Args) const;
	void DrawDebugLineTrace(const FSpawnStartArgs& Args, const FHitResult& Hit, const FVector& LineTraceStart, const FVector& LineTraceEnd,
		FColor Color, float Lifetime = 5.f, float Thickness = 5.f, float HitPointSize = 32.f) const;
	void AddNewSpawnedActor(AActor* SpawnedActor, int32 Index);

	UFUNCTION()
	void OnSpawnedActorDestroyed(AActor* SpawnedActor);
	
	int32 GetSpawnedCount(const TSubclassOf<AActor>& Spawned, int32 Index) const;
	
	UPROPERTY(EditAnywhere, Category=Spawner)
	bool bSpawnEnabled;
	
	UPROPERTY(EditAnywhere, Category=Spawner)
	TArray<FSpawnListEntry> SpawnList;

	UPROPERTY(VisibleAnywhere, Category=Spawner)
	TArray<float> DelaysList;

	UPROPERTY(VisibleAnywhere, Category=Spawner)
	int32 CurrentIndex;

	UPROPERTY(/*VisibleAnywhere, Category=Spawner*/)
	int32 CurrentCount_DEPRECATED;

	UPROPERTY(VisibleAnywhere, Category=Spawner)
	TArray<FSpawnedListEntry> SpawnedActors;

	FTimerHandle SpawnTimerHandle;
	FTimerDelegate Delegate;
};
