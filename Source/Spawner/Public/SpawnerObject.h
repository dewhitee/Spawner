// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerInterface.h"
#include "UObject/NoExportTypes.h"
#include "SpawnerObject.generated.h"

class USpawnListPreset;
USTRUCT(BlueprintType)
struct FRespawnListEntry_DEPRECATED
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawn, AActor*, SpawnedActor, const FSpawnArgs&, Args);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeferredSpawn, AActor*, SpawningActor, const FSpawnArgs&, Args);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpawnerIndexUpdated, int32, NewIndex, const FSpawnListEntry&, NewEntry);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreSpawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnerStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpawnerStop);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew, CollapseCategories, DisplayName="Spawner")
class SPAWNER_API USpawnerObject : public UObject, public ISpawnerInterface, public FTickableGameObject
{
	GENERATED_BODY()

public:
	USpawnerObject();
	
	// UObject
	virtual UWorld* GetWorld() const override;
	virtual void BeginDestroy() override;
	virtual void PostLoad() override;

	// FTickableGameObject
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	// ISpawnerInterface
	virtual void Start_Implementation(const FSpawnStartArgs& Args) override;
	virtual void Stop_Implementation() override;
	virtual AActor* Spawn_Implementation(const FSpawnArgs& Args) override;
	virtual AActor* Respawn_Implementation(const FSpawnArgs& Args) override;
	virtual bool IsRunning() const override;

	UFUNCTION(BlueprintCallable, Category=Spawner)
	bool IsSpawnEnabled() const;

	UFUNCTION(BlueprintCallable, Category=Spawner)
	void SetSpawnEnabled(bool bEnabled);

	UFUNCTION(BlueprintGetter, Category=Spawner)
	TArray<FSpawnListEntry> GetSpawnList() const;

	UFUNCTION(BlueprintCallable, Category=Spawner)
	void SetSpawnList(const TArray<FSpawnListEntry>& Entries);

	UFUNCTION(BlueprintCallable, Category=Spawner)
	void SetEditorOnlySpawnListData();

	UFUNCTION(BlueprintGetter)
	int32 GetCurrentIndex() const { return CurrentIndex; }

protected:
	void SnapToSurface(FVector& OutLocation, bool& bShouldSkip, const FSpawnStartArgs& Args) const;
	
private:
	FVector GetSpawnLocation(const FSpawnStartArgs& Args, bool& bShouldSkip) const;
	void DrawDebugLineTrace(const FSpawnStartArgs& Args, const FHitResult& Hit, const FVector& LineTraceStart, const FVector& LineTraceEnd,
		FColor Color, float Lifetime = 5.f, float Thickness = 5.f, float HitPointSize = 32.f) const;
	void DrawDebugLineTraceNoHitPoint(const FSpawnStartArgs& Args, const FVector& LineTraceStart, const FVector& LineTraceEnd,
		FColor Color, float Lifetime = 5.f, float Thickness = 5.f) const;
	void AddNewSpawnedActor(AActor* SpawnedActor, int32 Index);
	FVector GetLocationInRadius(const FSpawnStartArgs& Args, bool& bShouldSkip) const;

	UFUNCTION()
	void OnSpawnedActorDestroyed(AActor* SpawnedActor);
	
	int32 GetSpawnedCount(const TSubclassOf<AActor> Spawned, int32 Index, ESpawnCountCalculationMode CountCalculationMode) const;
	int32 GetTotalSpawnedCount() const;

	void StartDefault(const FSpawnStartArgs& Args);
	void DefaultSpawnTick(const FSpawnStartArgs Args); // Pass by copy, because FTimerDelegate does not allow refs
	void StartUsingDataTable(const FSpawnStartArgs& Args);
	void StartUsingCurveTable(const FSpawnStartArgs& Args);
	
	AActor* SpawnDefault(const FSpawnArgs& Args);
	AActor* SpawnUsingDataTable(const FSpawnArgs& Args);
	AActor* SpawnUsingCurveTable(const FSpawnArgs& Args);

	float GetCurrentCurveTableTime() const;
	float GetCurrentTime() const;

public:
	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawn OnSpawn;

	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnDeferredSpawn OnDeferredSpawn;
	
	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnPreSpawn OnPreSpawn;

	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawnerStop OnStop;
	
	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawnerStart OnStart;

	UPROPERTY(BlueprintAssignable, Category=Spawner)
	FOnSpawnerIndexUpdated OnIndexUpdated;
	
private:
	UPROPERTY(EditAnywhere, Category=Spawner)
	bool bSpawnEnabled;
	
	UPROPERTY(EditAnywhere, BlueprintGetter=GetSpawnList, Category=Spawner, meta=(TitleProperty="{ClassName}: Count={ActualCount}, Time={ActualTime}, TotalTime={TotalTime}"))
	TArray<FSpawnListEntry> SpawnList;

	/**
	 * Optional spawn list preset that will be loaded on spawner object Start method execution.
	 */
	UPROPERTY(EditAnywhere, Category=Spawner)
	TSoftObjectPtr<USpawnListPreset> SpawnListPreset;

	UPROPERTY(VisibleDefaultsOnly, BlueprintGetter=GetCurrentIndex, Category=Spawner, AdvancedDisplay)
	int32 CurrentIndex;

	UPROPERTY(/*VisibleAnywhere, Category=Spawner*/)
	int32 CurrentCount_DEPRECATED;

	UPROPERTY(VisibleAnywhere, Category=Spawner, AdvancedDisplay)
	TArray<FSpawnedListEntry> SpawnedActors;

	/**
	 * Is calculated in the Tick method or on each spawn based on the chosen mode (default/data table/curve table)
	 * - By default: Is incremented by DeltaTime in spawner's Tick method.
	 * - TODO: Curve table: CurveTableTickRate is used to increment passed time;
	 */
	UPROPERTY(VisibleAnywhere)
	float PassedTime;

	FTimerHandle SpawnTimerHandle;
	FTimerDelegate Delegate;
};
