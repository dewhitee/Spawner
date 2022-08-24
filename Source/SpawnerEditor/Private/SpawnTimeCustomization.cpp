// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnTimeCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "SpawnerObject.h"
#include "Widgets/Layout/SWrapBox.h"

#define LOCTEXT_NAMESPACE "SpawnerEditor"

TSharedRef<IPropertyTypeCustomization> FSpawnTimeCustomization::MakeInstance()
{
	return MakeShareable(new FSpawnTimeCustomization());
}

void FSpawnTimeCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
	IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> DelayPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnTime, Delay));
	const TSharedPtr<IPropertyHandle> RandomTimeScatterPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnTime, RandomTimeScatter));
	check(DelayPropertyHandle.IsValid());
	check(RandomTimeScatterPropertyHandle.IsValid());

	DelayPropertyHandle->GetValueAsDisplayText(DelayText);
	RandomTimeScatterPropertyHandle->GetValueAsDisplayText(RandomTimeScatterText);

	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(150.f)
	.MaxDesiredWidth(150.f)
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
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
							DelayPropertyHandle->CreatePropertyNameWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						DelayPropertyHandle->CreatePropertyValueWidget()
					]
				]
			]
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
							RandomTimeScatterPropertyHandle->CreatePropertyNameWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						RandomTimeScatterPropertyHandle->CreatePropertyValueWidget()
					]
				]
			]
		]
	];

	OnDelayChanged(DelayPropertyHandle);
	OnRandomTimeScatterChanged(RandomTimeScatterPropertyHandle);

	DelayPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnTimeCustomization::OnDelayChanged, DelayPropertyHandle));
	RandomTimeScatterPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnTimeCustomization::OnRandomTimeScatterChanged, RandomTimeScatterPropertyHandle));
}

void FSpawnTimeCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> DelayPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnTime, Delay));
	const TSharedPtr<IPropertyHandle> RandomTimeScatterPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnTime, RandomTimeScatter));

	/*ChildBuilder.AddCustomRow(LOCTEXT("FSpawnTimeRow", "FSpawnTime"))
	[
		DelayPropertyHandle->CreatePropertyValueWidget()
	];
	ChildBuilder.AddCustomRow(LOCTEXT("FSpawnTimeRow", "FSpawnTime"))
	[
		RandomTimeScatterPropertyHandle->CreatePropertyValueWidget()
	];*/

	/*ChildBuilder.AddCustomRow(LOCTEXT("FSpawnTimeRow", "FSpawnTime"))
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		.BorderBackgroundColor(FLinearColor(0.333f, 0.333f, 0.333))
		.Content()
		[
			SNew(SWrapBox)
			.UseAllottedWidth(true)
			+SWrapBox::Slot()
			.Padding(5.f, 0.f)
			[
				SNew(SBox)
				.MinDesiredWidth(70.f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						DelayPropertyHandle->CreatePropertyNameWidget()
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						DelayPropertyHandle->CreatePropertyValueWidget()
					]
				]
			]
			+SWrapBox::Slot()
			.Padding(5.f, 0.f)
			[
				SNew(SBox)
				.MinDesiredWidth(70.f)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						RandomTimeScatterPropertyHandle->CreatePropertyNameWidget()
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						RandomTimeScatterPropertyHandle->CreatePropertyValueWidget()
					]
				]
			]
		]
	];*/
}

void FSpawnTimeCustomization::OnDelayChanged(TSharedPtr<IPropertyHandle> DelayPropertyHandle)
{
	if (DelayPropertyHandle.IsValid() && DelayPropertyHandle->IsValidHandle())
	{
		DelayPropertyHandle->GetValueAsDisplayText(DelayText);
	}
}

void FSpawnTimeCustomization::OnRandomTimeScatterChanged(TSharedPtr<IPropertyHandle> RandomTimeScatterPropertyHandle)
{
	if (RandomTimeScatterPropertyHandle.IsValid() && RandomTimeScatterPropertyHandle->IsValidHandle())
	{
		RandomTimeScatterPropertyHandle->GetValueAsDisplayText(RandomTimeScatterText);
	}
}

#undef LOCTEXT_NAMESPACE
