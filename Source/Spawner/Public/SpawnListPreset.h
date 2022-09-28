// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "SpawnerObject.h"
#include "Engine/DataAsset.h"
#include "SpawnListPreset.generated.h"

UENUM()
enum class ESpawnListPresetDataSource : uint8
{
	Default,
	DataTable,
	CurveTable
};

class UDataTable;
/**
 * 
 */
UCLASS()
class SPAWNER_API USpawnListPreset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintGetter, Category=Spawner)
	TArray<FSpawnListEntry> GetSpawnList() const { return SpawnList; }

protected:
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
private:
	UPROPERTY(VisibleAnywhere, Category=Spawner, DisplayName="Total time", meta=(Units="Seconds", ShowOnlyInnerProperties))
	FSpawnStats Stats;

	UPROPERTY(EditAnywhere, Category=Spawner)
	ESpawnListPresetDataSource DataSource;
	
	UPROPERTY(EditAnywhere, Category=Spawner, meta=(EditCondition="DataSource == ESpawnListPresetDataSource::DataTable", EditConditionHides))
	TSoftObjectPtr<UDataTable> DataTable;

	UPROPERTY(EditAnywhere, Category=Spawner, meta=(EditCondition="DataSource == ESpawnListPresetDataSource::CurveTable", EditConditionHides))
	TSoftObjectPtr<UCurveTable> CurveTable;
	
	UPROPERTY(EditAnywhere, BlueprintGetter=GetSpawnList, Category=Spawner, meta=(TitleProperty="{ClassName}: Count={ActualCount}, Time={ActualTime}, TotalTime={TotalTime}"))
	TArray<FSpawnListEntry> SpawnList;
	
};
