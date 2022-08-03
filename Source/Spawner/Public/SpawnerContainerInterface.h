// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnerContainerInterface.generated.h"

class ISpawnerInterface;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawnerContainerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPAWNER_API ISpawnerContainerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	TScriptInterface<ISpawnerInterface> GetSpawner() const;
};
