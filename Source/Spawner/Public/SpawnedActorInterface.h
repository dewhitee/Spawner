// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnedActorInterface.generated.h"

class USpawnerObject;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawnedActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPAWNER_API ISpawnedActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, Category=Spawner)
	float GetCapsuleHalfHeight() const;

	virtual void SetSpawnerObject(USpawnerObject* InSpawnerObject) = 0;
	virtual USpawnerObject* GetSpawnerObject() const = 0;
};
