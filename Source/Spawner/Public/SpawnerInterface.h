// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "SpawnerInterface.generated.h"

class USpawnConditionObject;
class USpawnShape;

UENUM(BlueprintType)
enum class ESpawnMode : uint8
{
	None UMETA(Hidden),
	SpawnAlways,
	SpawnWhenPlayerIsNear,
	//SpawnWhenPlayerIsInsideShape,
	//SpawnWhenPlayerIsOutsideShape
};

USTRUCT(BlueprintType)
struct FSpawnTime
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(ClampMin=0.01, Delta=0.01, ForceUnits="s"))
	float Delay = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, DisplayName="Scatter", Category=Spawner, meta=(ClampMin=0, Delta=0.01, ForceUnits="s"))
	float RandomTimeScatter = 0.f;

	float Get() const { return Delay + FMath::RandRange(0.f, RandomTimeScatter); }

	bool operator==(const FSpawnTime& Rhs) const
	{
		return Delay == Rhs.Delay && RandomTimeScatter == Rhs.RandomTimeScatter;
	}
};

USTRUCT(BlueprintType)
struct FSpawnCount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bRandom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="!bRandom"))
	int32 ExactCount = 10;
	
	UPROPERTY(EditAnywhere, Category=Spawner, meta=(EditCondition="bRandom"))
	FInt32Range RandomCount = FInt32Range(0, 10);

	int32 Get() const
	{
		return bRandom
			? FMath::RandRange(RandomCount.GetLowerBoundValue(), RandomCount.GetUpperBoundValue())
			: ExactCount;
	}

	FString GetCountString() const
	{
		return bRandom && RandomCount.GetLowerBound().IsClosed() && RandomCount.GetUpperBound().IsClosed()
			? FString::Printf(TEXT("%s%d-%d%s"),
				RandomCount.GetLowerBound().IsInclusive() ? TEXT("[") : TEXT("("),
				RandomCount.GetLowerBoundValue(),
				RandomCount.GetUpperBoundValue(),
				RandomCount.GetUpperBound().IsInclusive() ? TEXT("]") : TEXT(")"))
			: FString::FromInt(ExactCount);
	}

	void ResetBounds()
	{
		if (RandomCount.GetLowerBound().IsOpen())
		{
			RandomCount.SetLowerBound(0);
		}
		if (RandomCount.GetUpperBound().IsOpen())
		{
			RandomCount.SetUpperBound(1);
		}
	}

	bool operator==(const FSpawnCount& Rhs) const
	{
		return bRandom == Rhs.bRandom && ExactCount == Rhs.ExactCount && RandomCount == Rhs.RandomCount;
	}
};

USTRUCT(BlueprintType)
struct FPostSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(ClampMin=0.01, Delta=0.01, ForceUnits="s"))
	float Delay = 0.f;
};

UENUM(BlueprintType)
enum class ESpawnConditionalValueMode : uint8
{
	Probability,
	EachIndex,
	Custom,
};

USTRUCT(BlueprintType)
struct SPAWNER_API FSpawnConditionalActorListEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TSoftClassPtr<AActor> ActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnConditionalValueMode ValueMode = ESpawnConditionalValueMode::Probability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(ClampMin=0.01, ClampMax=1.0))
	float Probability = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(ClampMin=1))
	int32 EachIndex = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category=Spawner)
	TObjectPtr<USpawnConditionObject> CustomCondition = nullptr;
};

USTRUCT(BlueprintType)
struct SPAWNER_API FSpawnConditional
{
	GENERATED_BODY()

	FSpawnConditional() {}
	FSpawnConditional(const TArray<FSpawnConditionalActorListEntry>& InActors) : Actors(InActors) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TArray<FSpawnConditionalActorListEntry> Actors;
};

USTRUCT(BlueprintType)
struct SPAWNER_API FSpawnListEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TSoftClassPtr<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FSpawnTime Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FSpawnCount Count;

	/**
	 * Array of actors that can spawn (with a specified probability) each time actor of the ClassToSpawn class is spawned.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, DisplayName="Conditionally Spawned Actors", meta=(ShowOnlyInnerProperties))
	TArray<FSpawnConditionalActorListEntry> ConditionalActors;
	// TODO: Replace with:	FSpawnConditional Conditional;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FPostSpawnData PostSpawnData;

#if WITH_EDITORONLY_DATA	
	/**
	 * Name of the class with no path information to on array entry. Only for display in TitleProperty meta specifier.
	 * 
	 * @warning Do not access outside of WITH_EDITOR
	 */
	UPROPERTY(VisibleAnywhere)
	FName ClassName;

	/**
	 * Actual count string of actors to spawn. Only for display in TitleProperty meta specifier.
	 * 
	 * @warning Do not access outside of WITH_EDITOR
	 */
	UPROPERTY(VisibleAnywhere)
	FName ActualCount;

	/**
	 * Actual spawn time string of actors to spawn. Only for display in TitleProperty meta specifier.
	 * 
	 * @warning Do not access outside of WITH_EDITOR
	 */
	UPROPERTY(VisibleAnywhere)
	FName ActualTime;

	/**
	 * Actual total spawn time string. Only for display in TitleProperty meta specifier.
	 * 
	 * @warning Do not access outside of WITH_EDITOR
	 */
	UPROPERTY(VisibleAnywhere)
	FName TotalTime;

	/**
	 * Actual total spawn time string. Only for display in TitleProperty meta specifier.
	 * 
	 * @warning Do not access outside of WITH_EDITOR
	 */
	UPROPERTY(VisibleAnywhere)
	FName ActualPostSpawnData;
#endif

	void SetEditorOnlyDisplayData();
	
	bool operator==(const FSpawnListEntry& Rhs) const
	{
		return ClassToSpawn == Rhs.ClassToSpawn && Time == Rhs.Time && Count == Rhs.Count;
	}
};

USTRUCT(BlueprintType)
struct SPAWNER_API FSpawnListEntryTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere, meta=(ClampMin=0.01, Delta=0.01, ForceUnits="s"))
	float Delay = 0.2f;

	UPROPERTY(EditAnywhere)
	int32 ExactCount = 10;
};

USTRUCT(BlueprintType)
struct FSpawnStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category=Spawner, DisplayName="Total time", meta=(Units="Seconds"))
	float TotalEntriesTime = 0.f;

	UPROPERTY(VisibleAnywhere, Category=Spawner, DisplayName="Total enemies spawned")
	int32 TotalEntriesEnemiesSpawned = 0;

	void Reset()
	{
		*this = {};
	}
	
	void AddFromEntry(const FSpawnListEntry& Entry)
	{
		TotalEntriesTime += Entry.Time.Get() * Entry.Count.Get();
		TotalEntriesEnemiesSpawned += Entry.Count.Get();
	}
};

USTRUCT(BlueprintType)
struct SPAWNER_API FSpawnedListEntry
{
	GENERATED_BODY()

	FSpawnedListEntry() : Index(0), SpawnedCount(0) {}
	FSpawnedListEntry(int32 InIndex, AActor* InActor) : Index(InIndex), SpawnedActors({InActor}), SpawnedCount(1)
	{
		SpawnedActorClass = InActor->GetClass();
	}
	
	TSubclassOf<AActor> GetClass() const
	{
		return /*SpawnedActors.IsEmpty() || !IsValid(SpawnedActors[0]) ? nullptr : SpawnedActors[0]->GetClass()*/SpawnedActorClass;
	}
	void AddSpawnedActor(AActor* NewActor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Spawned)
	int32 Index;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Spawned)
	TArray<TObjectPtr<AActor>> SpawnedActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Spawned)
	TSubclassOf<AActor> SpawnedActorClass;

	/**
	 * Will be used instead of SpawnedActors array if 
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Spawned)
	int32 SpawnedCount;
};

USTRUCT(BlueprintType)
struct FSpawnSnapToSurfaceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bSkipIfStillNotOnSurface = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bPushToRandomNavigableLocation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bUseRadiusAsSearchRadius = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="!bUseRadiusAsSearchRadius"))
	float NavigableLocationSearchRadius = 512.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bCorrectUsingNavProjection = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="bCorrectUsingNavProjection"))
	bool bRandomizeNavProjectionTargetLocation = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="bRandomizeNavProjectionTargetLocation"))
	float CorrectionRandomTargetLocationRadius = 64.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bAddCapsuleHalfHeight = false;
};

/*
UENUM(BlueprintType)
enum class ESpawnShape : uint8
{
	None UMETA(Hidden),
	Default,
	Circle
};*/

USTRUCT(BlueprintType)
struct FSpawnShapeSettings
{
	GENERATED_BODY()

	/**
	 * If shape is set, actors will be spawned according to this shape, e.g. inside of it.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	TObjectPtr<USpawnShape> SpawnShape = nullptr;
};

USTRUCT(BlueprintType)
struct FSpawnBehaviorSettings
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class ESpawnCountCalculationMode : uint8
{
	CurrentActorCount,
	EverSpawnedActorCount
};

/*USTRUCT(BlueprintType)
struct FSpawnCountSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnCountCalculationMode CountCalculationMode;
};*/

/*USTRUCT(BlueprintType)
struct FSpawnRadiusSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	float Radius = 100.f;

	/**
	 * Radius that is used to determine if Player is close enough to a spawner to start/continue spawning process.
	 * @note Is only used when SpawnMode is set to SpawnWhenPlayerIsNear
	 #1#
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="SpawnMode == ESpawnMode::SpawnWhenPlayerIsNear"))
	float SpawnEnabledRadius = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="SpawnMode == ESpawnMode::SpawnWhenPlayerIsNear"))
	bool bUseRadiusAsSpawnEnabledRadius = true;
};*/

USTRUCT(BlueprintType)
struct FSpawnStartArgs
{
	GENERATED_BODY()
	
	/**
	 * If set, location of an Actor holding the SpawnerObject will be used instead of a location specified in AtLocation parameter.
	 * 
	 * @note If both this and bUseRandomLocationInRadius is true, AtLocation parameter will be set to the location of the Outer actor
	 * and used in calculation of random location in radius.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bUseOuterLocation = false;
	
	/**
	 * Custom specified exact location at which to spawn the target actor.
	 * 
	 * @note Will be used if bUseOuterLocation is set but Outer object is not valid.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	mutable FVector AtLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	/**
	 * If set, random location X and Y components in specified Radius will be added to AtLocation argument.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bUseRandomLocationInRadius = false;

	/**
	 * Radius in which actors are spawned.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="bUseRandomLocationInRadius"))
	float Radius = 100.f;
	
	/**
	 * If set, spawned actors be snapped to the surface upon or underneath them.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bSnapToSurface = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="bSnapToSurface"))
	FSpawnSnapToSurfaceSettings SnapToSurfaceSettings;
	
	/**
	 * Do draw debug line traces when snapping to surface?
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnMode SpawnMode = ESpawnMode::SpawnAlways;

	/**
	 * Radius that is used to determine if Player is close enough to a spawner to start/continue spawning process.
	 * 
	 * @note Is only used when SpawnMode is set to SpawnWhenPlayerIsNear.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="SpawnMode == ESpawnMode::SpawnWhenPlayerIsNear && !bUseRadiusAsSpawnEnabledRadius"))
	mutable float SpawnEnabledRadius = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="SpawnMode == ESpawnMode::SpawnWhenPlayerIsNear"))
	bool bUseRadiusAsSpawnEnabledRadius = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FSpawnShapeSettings SpawnShapeSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bSetActorOwner = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="bSetActorOwner"))
	TObjectPtr<AActor> SpawnedActorOwner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bSetActorInstigator = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="bSetActorInstigator"))
	TObjectPtr<APawn> SpawnedActorInstigator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bDeferSpawn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnCountCalculationMode CountCalculationMode = ESpawnCountCalculationMode::CurrentActorCount;
	
	/**
	 * Should actors be respawned after if they were destroyed?
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bRespawnAfter = true;
};

USTRUCT(BlueprintType)
struct FSpawnArgs
{
	GENERATED_BODY()

	FSpawnArgs() {}
	FSpawnArgs(const FSpawnStartArgs& InArgs, FVector InLocation, const FSpawnListEntry& InEntry);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TSubclassOf<AActor> ClassToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FVector AtLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FRotator AtRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnActorCollisionHandlingMethod CollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bSetActorOwner = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="bSetActorOwner"))
	TObjectPtr<AActor> SpawnedActorOwner = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bSetActorInstigator = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner, meta=(EditCondition="bSetActorInstigator"))
	TObjectPtr<APawn> SpawnedActorInstigator = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bDeferSpawn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnCountCalculationMode CountCalculationMode = ESpawnCountCalculationMode::CurrentActorCount;

	UPROPERTY()
	FSpawnListEntry Entry;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpawnerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPAWNER_API ISpawnerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	void Start(const FSpawnStartArgs& Args);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	void Stop();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	AActor* Spawn(const FSpawnArgs& Args);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Spawner)
	AActor* Respawn(const FSpawnArgs& Args);
	
	virtual bool IsRunning() const = 0;
};
