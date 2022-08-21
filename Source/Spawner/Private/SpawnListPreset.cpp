// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnListPreset.h"

void USpawnListPreset::PostLoad()
{
	Super::PostLoad();

	Stats.Reset();
	for (FSpawnListEntry& Entry : SpawnList)
	{
		Stats.AddFromEntry(Entry);
		Entry.SetEditorOnlyDisplayData();
	}
}

#if WITH_EDITOR
void USpawnListPreset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property)
	{
		Stats.Reset();
		for (FSpawnListEntry& Entry : SpawnList)
		{
			Stats.AddFromEntry(Entry);
			Entry.SetEditorOnlyDisplayData();
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
