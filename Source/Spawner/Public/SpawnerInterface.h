// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnerInterface.generated.h"

UENUM(BlueprintType)
enum class ESpawnMode : uint8
{
	None UMETA(Hidden),
	SpawnAlways,
	SpawnWhenPlayerIsNear,
};

USTRUCT(BlueprintType)
struct FSpawnArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TSubclassOf<AActor> ClassToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FVector AtLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FRotator AtRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
};

USTRUCT(BlueprintType)
struct FSpawnStartArgs
{
	GENERATED_BODY()
	
	/**
	 * If set, location of an Actor holding the SpawnerObject will be used instead of a location specified in AtLocation parameter.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bUseOuterLocation = false;
	
	/**
	 * Custom specified exact location at which to spawn the target actor.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FVector AtLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	/**
	 * If set, random location X and Y components in specified Radius will be added to AtLocation argument.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bUseRandomLocationInRadius = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	float Radius = 100.f;
	
	/**
	 * If set, spawned actors be snapped to the surface upon or underneath them.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bSnapToSurface = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bSkipIfStillNotOnSurface = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnMode SpawnMode = ESpawnMode::SpawnAlways;

	/**
	 * Radius that is used to determine if Player is close enough to a spawner to start/continue spawning process.
	 * @note Is only used when SpawnMode is set to SpawnWhenPlayerIsNear
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	float SpawnEnabledRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bRespawnAfter = true;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawnerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPAWNER_API ISpawnerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	void Start(const FSpawnStartArgs& Args);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	void Stop();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	AActor* Spawn(const FSpawnArgs& Args);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	AActor* Respawn(const FSpawnArgs& Args);
	
};
