// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnerInterface.generated.h"

USTRUCT(BlueprintType)
struct FSpawnArgs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	TSubclassOf<AActor> ClassToSpawn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FVector AtLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FRotator AtRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnActorCollisionHandlingMethod CollisionHandlingMethod;
};

USTRUCT(BlueprintType)
struct FSpawnStartArgs
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bUseOuterLocation = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	FVector AtLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	ESpawnActorCollisionHandlingMethod CollisionHandlingMethod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	bool bUseRandomLocationInRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Spawner)
	float Radius;
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
	
};
