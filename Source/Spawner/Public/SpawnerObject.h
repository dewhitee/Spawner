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

	UPROPERTY(EditAnywhere, Category=Spawner, meta=(ClampMin=0))
	float Delay = 0.f;

	UPROPERTY(EditAnywhere, Category=Spawner, meta=(ClampMin=0))
	float RandomTimeScatter = 0.f;

	float Get() const { return Delay + FMath::RandRange(0.f, RandomTimeScatter); }
};

USTRUCT(BlueprintType)
struct FSpawnCount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Spawner)
	bool bRandom = false;

	UPROPERTY(EditAnywhere, Category=Spawner)
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

	UPROPERTY(EditAnywhere, Category=Spawner)
	TSubclassOf<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere, Category=Spawner)
	FSpawnTime Time;

	UPROPERTY(EditAnywhere, Category=Spawner, meta=(ClampMin=0))
	FSpawnCount Count;
};

USTRUCT(BlueprintType)
struct FRespawnListEntry
{
	GENERATED_BODY()
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawn, AActor*, SpawnedActor, const FSpawnArgs&, Args);
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
	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawn OnSpawn;

	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawnerStop OnStop;
	
	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawnerStart OnStart;
	
private:	
	UPROPERTY(EditAnywhere, Category=Spawner)
	bool bSpawnEnabled;
	
	UPROPERTY(EditAnywhere, Category=Spawner)
	TArray<FSpawnListEntry> SpawnList;

	UPROPERTY(VisibleAnywhere, Category=Spawner)
	TArray<float> DelaysList;

	UPROPERTY(VisibleAnywhere, Category=Spawner)
	int32 CurrentIndex;

	UPROPERTY(VisibleAnywhere, Category=Spawner)
	int32 CurrentCount;

	FTimerHandle SpawnTimerHandle;
	FTimerDelegate Delegate;
	
public:
	virtual UWorld* GetWorld() const override;
	
	// ISpawnerInterface
	virtual void Start_Implementation(const FSpawnStartArgs& Args) override;
	virtual void Stop_Implementation() override;
	virtual AActor* Spawn_Implementation(const FSpawnArgs& Args) override;
	virtual AActor* Respawn_Implementation(const FSpawnArgs& Args) override;

private:
	FVector GetSpawnLocation(const FSpawnStartArgs& Args) const;
};
