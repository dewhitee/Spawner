// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

/**
 * 
 */
class SPAWNEREDITOR_API FSpawnCountCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	FText Text_bRandom;
	FText Text_Count;
	FText Text_RandomCount;

	void OnbRandomChanged(TSharedPtr<IPropertyHandle> PropertyHandle_bRandom);
	void OnCountChanged(TSharedPtr<IPropertyHandle> PropertyHandle_Count);
	void OnRandomCountChanged(TSharedPtr<IPropertyHandle> PropertyHandle_RandomCount);
};
