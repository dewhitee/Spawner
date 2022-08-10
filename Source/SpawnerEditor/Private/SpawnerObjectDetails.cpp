// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnerObjectDetails.h"

#include "DetailLayoutBuilder.h"
#include "ParameterizedSpawnerObject.h"
#include "SpawnerObject.h"

void FSpawnerObjectDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	if (Objects.Num() != 1)
	{
		return;
	}

	TWeakObjectPtr<UParameterizedSpawnerObject> Spawner = Cast<UParameterizedSpawnerObject>(Objects[0].Get());
	if (!Spawner.IsValid())
	{
		return;
	}
	
	// TODO: implement
	//DetailBuilder.HideProperty()
	TSharedRef<IPropertyHandle> SpawnSettingsRadiusProperty = DetailBuilder.GetProperty(TEXT("SpawnSettings.Radius"));
	//if (SpawnSettingsRadiusProperty->IsEditable())
}

TSharedRef<IDetailCustomization> FSpawnerObjectDetails::MakeInstance()
{
	return MakeShareable(new FSpawnerObjectDetails);
}
