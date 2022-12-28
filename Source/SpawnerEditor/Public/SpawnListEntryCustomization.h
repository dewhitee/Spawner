// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SPAWNEREDITOR_API FSpawnListEntryCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

protected:
	TSharedPtr<IPropertyHandle> ClassToSpawnPropertyHandle;
	TSharedPtr<IPropertyHandle> TimePropertyHandle;
	TSharedPtr<IPropertyHandle> CountPropertyHandle;
	TSharedPtr<IPropertyHandle> ConditionalPropertyHandle;
	TSharedPtr<IPropertyHandle> ConditionalActorsPropertyHandle;
	TSharedPtr<IPropertyHandle> PostSpawnDataPropertyHandle;
	TSharedPtr<IPropertyHandle> ClassNamePropertyHandle;
	TSharedPtr<IPropertyHandle> ActualCountPropertyHandle;
	TSharedPtr<IPropertyHandle> ActualTimePropertyHandle;
	TSharedPtr<IPropertyHandle> ActualPostSpawnDataPropertyHandle;
	
	FText ClassToSpawnText;
	FText TimeText;
	FText CountText;
	FText ConditionalText;
	FText ConditionalActorsText;
	FText PostSpawnDataText;
	FText ClassNameText;
	FText ActualCountText;
	FText ActualTimeText;
	FText ActualPostSpawnDataText;

	void OnClassToSpawnChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnTimeChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnCountChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnConditionalChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnConditionalActorsChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnPostSpawnDataChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnClassNameChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnActualCountChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnActualTimeChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
	void OnActualPostSpawnDataChanged(TSharedPtr<IPropertyHandle> PropertyHandle);
};
