// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnConditionalActorListEntryCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "SpawnerInterface.h"

TSharedRef<IPropertyTypeCustomization> FSpawnConditionalActorListEntryCustomization::MakeInstance()
{
	return MakeShareable(new FSpawnConditionalActorListEntryCustomization());
}

void FSpawnConditionalActorListEntryCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> ActorClassPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnConditionalActorListEntry, ActorClass));
	const TSharedPtr<IPropertyHandle> ValueModePropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnConditionalActorListEntry, ValueMode));
	const TSharedPtr<IPropertyHandle> ProbabilityPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnConditionalActorListEntry, Probability));
	const TSharedPtr<IPropertyHandle> EachIndexPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnConditionalActorListEntry, EachIndex));
	const TSharedPtr<IPropertyHandle> CustomPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnConditionalActorListEntry, CustomCondition));
	
	check(ActorClassPropertyHandle.IsValid());
	check(ValueModePropertyHandle.IsValid());
	check(ProbabilityPropertyHandle.IsValid());
	check(EachIndexPropertyHandle.IsValid());
	check(CustomPropertyHandle.IsValid());

	ActorClassPropertyHandle->GetValueAsDisplayText(Text_ActorClass);
	ValueModePropertyHandle->GetValueAsDisplayText(Text_ValueMode);
	ProbabilityPropertyHandle->GetValueAsDisplayText(Text_Probability);
	EachIndexPropertyHandle->GetValueAsDisplayText(Text_EachIndex);
	CustomPropertyHandle->GetValueAsDisplayText(Text_Custom);

	uint8 ValueAsByte;
	ValueModePropertyHandle->GetValue(ValueAsByte);
	ValueMode = static_cast<ESpawnConditionalValueMode>(ValueAsByte);

	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(186.f)
	//.MaxDesiredWidth(240.f)
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
						.MinDesiredWidth(90.f)
						[
							ActorClassPropertyHandle->CreatePropertyNameWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						ActorClassPropertyHandle->CreatePropertyValueWidget(false)
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
						.MinDesiredWidth(90.f)
						[
							ValueModePropertyHandle->CreatePropertyValueWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						//ValueModePropertyHandle->CreatePropertyValueWidget(false)
						SNew(SBox)
						.IsEnabled(MakeAttributeLambda([=]{ return ValueMode == ESpawnConditionalValueMode::Probability; }))
                        .Visibility_Lambda([=]{ return ValueMode == ESpawnConditionalValueMode::Probability ? EVisibility::Visible : EVisibility::Collapsed; })
						[
							ProbabilityPropertyHandle->CreatePropertyValueWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SBox)
						.IsEnabled(MakeAttributeLambda([=]{ return ValueMode == ESpawnConditionalValueMode::EachIndex; }))
						.Visibility_Lambda([=]{ return ValueMode == ESpawnConditionalValueMode::EachIndex ? EVisibility::Visible : EVisibility::Collapsed; })
						[
							EachIndexPropertyHandle->CreatePropertyValueWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SBox)
						.IsEnabled(MakeAttributeLambda([=]{ return ValueMode == ESpawnConditionalValueMode::Custom; }))
						.Visibility_Lambda([=]{ return ValueMode == ESpawnConditionalValueMode::Custom ? EVisibility::Visible : EVisibility::Collapsed; })
						[
							CustomPropertyHandle->CreatePropertyValueWidget()
						]
					]
				]
			]
			/*+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.IsEnabled(MakeAttributeLambda([=]{ return ValueMode == ESpawnConditionalValueMode::Probability; }))
				.Visibility_Lambda([=]{ return ValueMode == ESpawnConditionalValueMode::Probability ? EVisibility::Visible : EVisibility::Collapsed; })
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.MinDesiredWidth(72.f)
						[
							ProbabilityPropertyHandle->CreatePropertyNameWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						ProbabilityPropertyHandle->CreatePropertyValueWidget()
					]
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SBox)
				.IsEnabled(MakeAttributeLambda([=]{ return ValueMode == ESpawnConditionalValueMode::EachIndex; }))
				.Visibility_Lambda([=]
				{
					return ValueMode == ESpawnConditionalValueMode::EachIndex ? EVisibility::Visible : EVisibility::Collapsed;
				})
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
						.MinDesiredWidth(72.f)
						[
							EachIndexPropertyHandle->CreatePropertyNameWidget()
						]
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						EachIndexPropertyHandle->CreatePropertyValueWidget()
					]
				]
			]*/
		]
	];

	OnActorClassChanged(ActorClassPropertyHandle);
	OnValueModeChanged(ValueModePropertyHandle);
	OnProbabilityChanged(ProbabilityPropertyHandle);
	OnEachIndexChanged(EachIndexPropertyHandle);
	OnCustomChanged(CustomPropertyHandle);
	
	ActorClassPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnConditionalActorListEntryCustomization::OnActorClassChanged, ActorClassPropertyHandle));
	ValueModePropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnConditionalActorListEntryCustomization::OnValueModeChanged, ValueModePropertyHandle));
	ProbabilityPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnConditionalActorListEntryCustomization::OnProbabilityChanged, ProbabilityPropertyHandle));
	EachIndexPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnConditionalActorListEntryCustomization::OnEachIndexChanged, EachIndexPropertyHandle));
	CustomPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnConditionalActorListEntryCustomization::OnCustomChanged, CustomPropertyHandle));
}

void FSpawnConditionalActorListEntryCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	
}

void FSpawnConditionalActorListEntryCustomization::OnActorClassChanged(
	TSharedPtr<IPropertyHandle> PropertyHandle_ActorClass)
{
	if (PropertyHandle_ActorClass.IsValid() && PropertyHandle_ActorClass->IsValidHandle())
	{
		PropertyHandle_ActorClass->GetValueAsDisplayText(Text_ActorClass);
	}
}

void FSpawnConditionalActorListEntryCustomization::OnValueModeChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(Text_ValueMode);
		uint8 ValueAsByte;
		PropertyHandle->GetValue(ValueAsByte);
		ValueMode = static_cast<ESpawnConditionalValueMode>(ValueAsByte);
	}
}

void FSpawnConditionalActorListEntryCustomization::OnProbabilityChanged(
	TSharedPtr<IPropertyHandle> PropertyHandle_Probability)
{
	if (PropertyHandle_Probability.IsValid() && PropertyHandle_Probability->IsValidHandle())
	{
		PropertyHandle_Probability->GetValueAsDisplayText(Text_Probability);
	}
}

void FSpawnConditionalActorListEntryCustomization::OnEachIndexChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(Text_EachIndex);
	}
}

void FSpawnConditionalActorListEntryCustomization::OnCustomChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(Text_Custom);
	}
}
