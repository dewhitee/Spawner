// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpawnerFunctionLibrary.generated.h"

class UParameterizedSpawnerObject;
class USpawnerObject;

/**
 * 
 */
UCLASS()
class SPAWNER_API USpawnerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category=Spawner)
	static void PushToSpawn(UParameterizedSpawnerObject* Spawner, AActor* ActorToPush, float DistanceThreshold = 2048.f);
};
