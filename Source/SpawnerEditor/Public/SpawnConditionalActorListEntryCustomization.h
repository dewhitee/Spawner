// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "SpawnerInterface.h"

/**
 * 
 */
class SPAWNEREDITOR_API FSpawnConditionalActorListEntryCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	FText Text_ActorClass;
	FText Text_ValueMode;
	FText Text_Probability;
	FText Text_EachIndex;
	FText Text_Custom;

	ESpawnConditionalValueMode ValueMode = ESpawnConditionalValueMode::Probability;

	void OnActorClassChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnValueModeChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnProbabilityChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnEachIndexChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnCustomChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
};
