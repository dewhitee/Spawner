// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnConditionObject.generated.h"

/**
 * Decides if passed in CanSpawn method ClassToSpawn can be spawned.
 * 
 * @see USpawnConditionObject::CanSpawn
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class SPAWNER_API USpawnConditionObject : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Checks if we can spawn an actor of a ClassToSpawn class.
	 * @param ClassToSpawn Class that we want to spawn.
	 * @param SpawnedMain Already spawned main class, guaranteed to be valid at the time of this method execution.
	 * @param CurrentEntryIndex Index of an entry structure that was responsible for spawning SpawnedMain actor.
	 * @param CurrentSpawnedCount Count of current entry spawned main actors.
	 * @param TotalSpawnedCount Total count of spawned main actors, not including those that were destroyed after spawn.
	 * @return True if ClassToSpawn can be spawned with given data, false otherwise.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category=Spawner)
	bool CanSpawn(TSubclassOf<AActor> ClassToSpawn, AActor* SpawnedMain, int32 CurrentEntryIndex, int32 CurrentSpawnedCount, int32 TotalSpawnedCount) const;
};
