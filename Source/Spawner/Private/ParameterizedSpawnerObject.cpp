// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "ParameterizedSpawnerObject.h"

void UParameterizedSpawnerObject::Start_Implementation(const FSpawnStartArgs& Args)
{
	Super::Start_Implementation(SpawnSettings);
}

void UParameterizedSpawnerObject::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// TODO: Update shape params
	/*if (PropertyChangedEvent.Property)
	{
		if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UParameterizedSpawnerObject, SpawnSettings))
		{
			
		}
	}*/
}

void UParameterizedSpawnerObject::SetAtLocation(FVector AtLocation)
{
	SpawnSettings.AtLocation = AtLocation;
}

void UParameterizedSpawnerObject::SetRadius(float Radius)
{
	SpawnSettings.Radius = Radius;
	if (SpawnSettings.bUseRadiusAsSpawnEnabledRadius)
	{
		SpawnSettings.SpawnEnabledRadius = SpawnSettings.Radius;
	}
}

FSpawnStartArgs UParameterizedSpawnerObject::GetSpawnSettings() const
{
	return SpawnSettings;
}

void UParameterizedSpawnerObject::SetSpawnSettings(const FSpawnStartArgs& InSpawnSettings)
{
	SpawnSettings = InSpawnSettings;
}
