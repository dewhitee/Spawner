// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnConditionalCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
#include "SpawnerInterface.h"

TSharedRef<IPropertyTypeCustomization> FSpawnConditionalCustomization::MakeInstance()
{
	return MakeShareable(new FSpawnConditionalCustomization());
}

void FSpawnConditionalCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> ActorsPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnConditional, Actors));
	check(ActorsPropertyHandle.IsValid());

	ActorsPropertyHandle->GetValueAsDisplayText(Text_Actors);
	
	//TSharedRef<FDetailArrayBuilder> ActorArrayBuilder = MakeShareable(new FDetailArrayBuilder(ActorsPropertyHandle.ToSharedRef()));
	//ActorArrayBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(this, &FSpawnConditionalCustomization::GenerateActorArrayEntryWidget));
	//ChildBuilder.AddCustomBuilder(ActorArrayBuilder);

	//ActorsArrayBuilder = MakeShareable(new FDetailArrayBuilder(ActorsPropertyHandle.ToSharedRef()));
	//ActorsArrayBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(this, &FSpawnConditionalCustomization::GenerateActorArrayEntryWidget));
	//ActorsArrayBuilder->GenerateHeaderRowContent(HeaderRow);

	HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	[
		PropertyHandle->CreatePropertyValueWidget()
	];
	
	OnActorsChanged(ActorsPropertyHandle);
	ActorsPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnConditionalCustomization::OnActorsChanged, ActorsPropertyHandle));
}

void FSpawnConditionalCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	const TSharedPtr<IPropertyHandle> ActorsPropertyHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnConditional, Actors));
	ActorsArrayBuilder = MakeShareable(new FDetailArrayBuilder(ActorsPropertyHandle.ToSharedRef()));
	ActorsArrayBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(this, &FSpawnConditionalCustomization::GenerateActorArrayEntryWidget));
	ChildBuilder.AddCustomBuilder(ActorsArrayBuilder.ToSharedRef());
	
	//ActorsArrayBuilder->GenerateChildContent(ChildBuilder);
	//ActorsArrayBuilder->RefreshChildren()
	//ActorArrayBuilder->OnGenerateArrayElementWidget(FOnGenerateArrayElementWidget::CreateSP(this, &FSpawnConditionalCustomization::GenerateActorArrayEntryWidget));
	//ChildBuilder.AddCustomBuilder(ActorsArrayBuilder.ToSharedRef());
}

void FSpawnConditionalCustomization::OnActorsChanged(TSharedPtr<IPropertyHandle> PropertyHandle_Actors)
{
	if (PropertyHandle_Actors.IsValid() && PropertyHandle_Actors->IsValidHandle())
	{
		PropertyHandle_Actors->GetValueAsDisplayText(Text_Actors);
		//const TSharedRef<FDetailArrayBuilder> ActorArrayBuilder = MakeShareable(new FDetailArrayBuilder(PropertyHandle_Actors.ToSharedRef()));
		//ActorArrayBuilder->RefreshChildren();
	}
}

void FSpawnConditionalCustomization::GenerateActorArrayEntryWidget(TSharedRef<IPropertyHandle> PropertyHandle,
	int32 ArrayIndex, IDetailChildrenBuilder& ChildrenBuilder)
{
	IDetailPropertyRow& Row = ChildrenBuilder.AddProperty(PropertyHandle);
}
