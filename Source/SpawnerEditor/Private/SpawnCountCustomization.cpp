// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnCountCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "SpawnerObject.h"

#define LOCTEXT_NAMESPACE "SpawnerEditor"

TSharedRef<IPropertyTypeCustomization> FSpawnCountCustomization::MakeInstance()
{
	return MakeShareable(new FSpawnCountCustomization());
}

void FSpawnCountCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> PropertyHandle_bRandom = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnCount, bRandom));
	const TSharedPtr<IPropertyHandle> PropertyHandle_Count = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnCount, ExactCount));
	const TSharedPtr<IPropertyHandle> PropertyHandle_RandomCount = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnCount, RandomCount));
	
	check(PropertyHandle_bRandom.IsValid());
	check(PropertyHandle_Count.IsValid());
	check(PropertyHandle_RandomCount.IsValid());

	PropertyHandle_bRandom->GetValueAsDisplayText(Text_bRandom);
	PropertyHandle_Count->GetValueAsDisplayText(Text_Count);
	PropertyHandle_RandomCount->GetValueAsDisplayText(Text_RandomCount);

	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(150.f)
	.MaxDesiredWidth(150.f)
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.BorderBackgroundColor(FLinearColor(1.f, 1.f, 1.f, 0.f))
		.Padding(5.f)
		.Content()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.MinDesiredWidth(72.f)
						[
							PropertyHandle_Count->CreatePropertyNameWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						PropertyHandle_Count->CreatePropertyValueWidget()
					]
				]
			]
		]
	];

	OnbRandomChanged(PropertyHandle_bRandom);
	OnCountChanged(PropertyHandle_Count);
	OnRandomCountChanged(PropertyHandle_RandomCount);

	PropertyHandle_bRandom->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnCountCustomization::OnbRandomChanged, PropertyHandle_bRandom));
	PropertyHandle_Count->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnCountCustomization::OnCountChanged, PropertyHandle_Count));
	PropertyHandle_RandomCount->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnCountCustomization::OnRandomCountChanged, PropertyHandle_RandomCount));
}

void FSpawnCountCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> PropertyHandle_bRandom = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnCount, bRandom));
	ChildBuilder.AddProperty(PropertyHandle_bRandom.ToSharedRef());
	
	const TSharedPtr<IPropertyHandle> RandomCountPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnCount, RandomCount));
	ChildBuilder.AddProperty(RandomCountPropertyHandle.ToSharedRef());
}

void FSpawnCountCustomization::OnbRandomChanged(TSharedPtr<IPropertyHandle> PropertyHandle_bRandom)
{
	if (PropertyHandle_bRandom.IsValid() && PropertyHandle_bRandom->IsValidHandle())
	{
		PropertyHandle_bRandom->GetValueAsDisplayText(Text_bRandom);
	}
}

void FSpawnCountCustomization::OnCountChanged(TSharedPtr<IPropertyHandle> PropertyHandle_Count)
{
	if (PropertyHandle_Count.IsValid() && PropertyHandle_Count->IsValidHandle())
	{
		PropertyHandle_Count->GetValueAsDisplayText(Text_Count);
	}
}

void FSpawnCountCustomization::OnRandomCountChanged(TSharedPtr<IPropertyHandle> PropertyHandle_RandomCount)
{
	if (PropertyHandle_RandomCount.IsValid() && PropertyHandle_RandomCount->IsValidHandle())
	{
		PropertyHandle_RandomCount->GetValueAsDisplayText(Text_RandomCount);
	}
}

#undef LOCTEXT_NAMESPACE