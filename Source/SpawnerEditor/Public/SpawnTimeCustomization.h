// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

/**
 * 
 */
class SPAWNEREDITOR_API FSpawnTimeCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	FText DelayText;
	FText RandomTimeScatterText;

	void OnDelayChanged(TSharedPtr<IPropertyHandle> DelayPropertyHandle);
	void OnRandomTimeScatterChanged(TSharedPtr<IPropertyHandle> RandomTimeScatterPropertyHandle);
};
