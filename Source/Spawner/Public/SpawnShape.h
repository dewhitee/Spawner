// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SpawnShape.generated.h"

class UParameterizedSpawnerObject;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, DefaultToInstanced, EditInlineNew)
class SPAWNER_API USpawnShape : public UObject
{
	GENERATED_BODY()

public:
	virtual FVector GetLocationInShape() const { unimplemented(); return {}; }
};

UCLASS()
class SPAWNER_API URingSpawnShape : public USpawnShape
{
	GENERATED_BODY()

public:
	URingSpawnShape();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual FVector GetLocationInShape() const override;
	float GetOuterRadius() const { return OuterRadius; }
	float GetInnerRadius() const { return InnerRadius; }

	UParameterizedSpawnerObject* GetSpawner() const;
	
private:
	UPROPERTY(EditAnywhere, Category=Shape, meta=(ClampMin=1))
	float OuterRadius;

	UPROPERTY(EditAnywhere, Category=Shape, meta=(ClampMin=1))
	float InnerRadius;
};