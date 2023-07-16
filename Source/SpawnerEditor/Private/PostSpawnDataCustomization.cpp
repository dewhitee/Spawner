// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "PostSpawnDataCustomization.h"

#include "DetailWidgetRow.h"
#include "SpawnConditionalActorListEntryCustomization.h"

TSharedRef<IPropertyTypeCustomization> FPostSpawnDataCustomization::MakeInstance()
{
	return MakeShareable(new FPostSpawnDataCustomization());
}

void FPostSpawnDataCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> PropertyHandle_Delay = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPostSpawnData, Delay));
	check(PropertyHandle_Delay);

	PropertyHandle_Delay->GetValueAsDisplayText(Text_Delay);

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
							PropertyHandle_Delay->CreatePropertyNameWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						PropertyHandle_Delay->CreatePropertyValueWidget()
					]
				]
			]
		]
	];
}

void FPostSpawnDataCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

void FPostSpawnDataCustomization::OnDelayChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(Text_Delay);
	}
}
