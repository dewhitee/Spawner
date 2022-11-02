// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
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

/*USTRUCT(BlueprintType)
struct FSpawnListCurveTableEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TSoftClassPtr<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere)
	FScalableFloat CountOverTime;
};*/

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

	ESpawnListPresetDataSource GetDataSource() const { return DataSource; }
	UCurveTable* GetCurveTable() const;
	float GetCurveTableTickRate() const;

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

	// Data table
	UPROPERTY(EditAnywhere, Category="Spawner Data Table", meta=(EditCondition="DataSource == ESpawnListPresetDataSource::DataTable", EditConditionHides))
	TSoftObjectPtr<UDataTable> DataTable;

	// Curve table
	UPROPERTY(EditAnywhere, Category="Spawner Curve Table", meta=(EditCondition="DataSource == ESpawnListPresetDataSource::CurveTable", EditConditionHides))
	TSoftObjectPtr<UCurveTable> CurveTable;

	UPROPERTY(EditAnywhere, Category="Spawner Curve Table", meta=(EditCondition="DataSource == ESpawnListPresetDataSource::CurveTable", EditConditionHides))
	bool bRefreshCurveTableEntries = true;

	UPROPERTY(EditAnywhere, Category="Spawner Curve Table", meta=(EditCondition="DataSource == ESpawnListPresetDataSource::CurveTable", EditConditionHides))
	float CurveTableTickRate = 1.f;

	UPROPERTY(EditAnywhere, Category="Spawner Curve Table", meta=(EditCondition="DataSource == ESpawnListPresetDataSource::CurveTable", EditConditionHides))
	TMap<TSoftClassPtr<AActor>, FScalableFloat> CurveTableEntries;

	// Default
	UPROPERTY(EditAnywhere, BlueprintGetter=GetSpawnList, Category=Spawner, meta=(EditCondition="DataSource == ESpawnListPresetDataSource::Default", TitleProperty="{ClassName}: Count={ActualCount}, Time={ActualTime}, TotalTime={TotalTime}"))
	TArray<FSpawnListEntry> SpawnList;
	
};
