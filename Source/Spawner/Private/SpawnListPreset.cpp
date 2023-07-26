// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnListPreset.h"

UCurveTable* USpawnListPreset::GetCurveTable() const
{
	return CurveTable.LoadSynchronous();
}

float USpawnListPreset::GetCurveTableTickRate() const
{
	return CurveTableTickRate;
}

TMap<TSoftClassPtr<AActor>, FScalableFloat> USpawnListPreset::GetCurveTableEntries() const
{
	return CurveTableEntries;
}

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

		switch (DataSource)
		{
		case ESpawnListPresetDataSource::Default:
			break;
			
		case ESpawnListPresetDataSource::DataTable:
			break;
			
		// Curve table entries update
		case ESpawnListPresetDataSource::CurveTable:
			if (!bRefreshCurveTableEntries || !CurveTable.LoadSynchronous())
			{
				break;
			}
			
			for (auto& Entry : CurveTableEntries)
			{
				//Entry.Value.SetValue(1.f);
				Entry.Value.Curve.CurveTable = CurveTable.Get();
			}
			break;
			
		default: ;
		}
	}
	
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
