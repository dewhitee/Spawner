// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "SpawnerObject.h"
#include "ParameterizedSpawnerObject.generated.h"

/**
 * Spawner that has editable spawn settings automatically passed to Start method on spawning.
 */
UCLASS(DisplayName="Parameterized Spawner")
class SPAWNER_API UParameterizedSpawnerObject : public USpawnerObject
{
	GENERATED_BODY()

public:
	// USpawnerObject
	virtual void Start_Implementation(const FSpawnStartArgs& Args) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void SetAtLocation(FVector AtLocation);
	void SetRadius(float Radius);

	FSpawnStartArgs GetSpawnSettings() const;
	void SetSpawnSettings(const FSpawnStartArgs& InSpawnSettings);

private:
	UPROPERTY(EditAnywhere, Category=Spawner, meta=(DisplayAfter="SpawnedActors"))
	FSpawnStartArgs SpawnSettings;
};
