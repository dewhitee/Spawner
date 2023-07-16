// Copyright (c) dewhitee (dewhiteeofficial@gmail.com)


#include "SpawnListEntryCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "SpawnerEditor.h"
#include "SpawnerObject.h"

#define LOCTEXT_NAMESPACE "SpawnerEditor"

TSharedRef<IPropertyTypeCustomization> FSpawnListEntryCustomization::MakeInstance()
{
	return MakeShareable(new FSpawnListEntryCustomization());
}

void FSpawnListEntryCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ClassToSpawnPropertyHandle			= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, ClassToSpawn));
	TimePropertyHandle					= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, Time));
	CountPropertyHandle					= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, Count));
	//ConditionalPropertyHandle	= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, Conditional));
	ConditionalActorsPropertyHandle		= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, ConditionalActors));
	PostSpawnDataPropertyHandle			= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, PostSpawnData));
	ClassNamePropertyHandle				= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, ClassName));
	ActualCountPropertyHandle			= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, ActualCount));
	ActualTimePropertyHandle			= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, ActualTime));
	ActualPostSpawnDataPropertyHandle	= PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSpawnListEntry, ActualPostSpawnData));

	ClassToSpawnPropertyHandle->GetValueAsDisplayText(ClassToSpawnText);
	TimePropertyHandle->GetValueAsDisplayText(TimeText);
	CountPropertyHandle->GetValueAsDisplayText(CountText);
	//ConditionalPropertyHandle->GetValueAsDisplayText(ConditionalText);
	ConditionalActorsPropertyHandle->GetValueAsDisplayText(ConditionalActorsText);
	PostSpawnDataPropertyHandle->GetValueAsDisplayText(PostSpawnDataText);
	ClassNamePropertyHandle->GetValueAsDisplayText(ClassNameText);
	ActualCountPropertyHandle->GetValueAsDisplayText(ActualCountText);
	ActualTimePropertyHandle->GetValueAsDisplayText(ActualTimeText);
	ActualPostSpawnDataPropertyHandle->GetValueAsDisplayText(ActualPostSpawnDataText);
	
	HeaderRow.WholeRowContent()
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		.BorderBackgroundColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.f))
		.Padding(5.f)
		.Content()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.MaxWidth(100.f)
			[
				PropertyHandle->CreatePropertyNameWidget()
			]
			+SHorizontalBox::Slot()
			.Padding(5.f)
			.AutoWidth()
			[
				PropertyHandle->CreatePropertyValueWidget()
				//SNew(STextBlock)
				//.Text_Lambda([this, PropertyHandle]
				//{
				//	OnClassNameChanged(ClassNamePropertyHandle);
				//	OnActualCountChanged(ActualCountPropertyHandle);
				//	OnActualTimeChanged(ActualTimePropertyHandle);
//
				//	// TODO: Get values of count and time to display total time required for spawn list entry to pass
				//	/*FProperty* CountProperty = CountPropertyHandle->GetProperty();
				//	UE_LOG(LogSpawnerEditor, Log, TEXT("CountProperty=%s, CountProperty->GetClass()=%s, CountText=%s"), *CountProperty->GetName(), *CountProperty->GetClass()->GetName(), *CountText.ToString());
				//	UE_LOG(LogSpawnerEditor, Log, TEXT("CountProperty->GetOwnerStruct()=%s"), CountProperty->GetOwnerStruct() ? *CountProperty->GetOwnerStruct()->GetName() : TEXT("None"));
				//	//int32* Value = CountProperty->ContainerPtrToValuePtr<int32>(CountProperty);
				//	FStructProperty* StructProperty = CastFieldChecked<FStructProperty>(CountProperty);
				//	for (TFieldIterator<FProperty> It(StructProperty->Struct); It; ++It)
				//	{
				//		UE_LOG(LogSpawnerEditor, Log, TEXT("It=%s"), *It->GetName());
				//		FProperty* TargetProperty = *It;
				//		if (TargetProperty->GetName() == GET_MEMBER_NAME_STRING_CHECKED(FSpawnCount, ExactCount))
				//		{
				//			TArray<UObject*> OuterObjects;
				//			PropertyHandle->GetOuterObjects(OuterObjects);
				//			check(!OuterObjects.IsEmpty());
				//			check(OuterObjects[0]);
				//			
				//			USpawnerObject* SpawnerObject = Cast<USpawnerObject>(OuterObjects[0]);
				//			for (int32 i = 0; i < OuterObjects.Num(); i++)
				//			{
				//				UE_LOG(LogSpawnerEditor, Log, TEXT("OuterObject[%d]=%s"), i, *OuterObjects[i]->GetName());
				//			}
				//			// TODO: This property can be outside of USpawnerObject, in that case we should refactor this
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("SpawnerObject=%s"), *SpawnerObject->GetName());
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty=%s"), StructProperty ? *StructProperty->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->GetTypedOwner<UObject>()=%s"), StructProperty->GetTypedOwner<UObject>() ? *StructProperty->GetTypedOwner<UObject>()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->OwnerUField=%s"), StructProperty->GetOwnerUField() ? *StructProperty->GetOwnerUField()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->OwnerUField->Outer=%s"), StructProperty->GetOwnerUField()->GetOuter() ? *StructProperty->GetOwnerUField()->GetOuter()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->OwnerUField->ActorOuter=%s"), StructProperty->GetOwnerUField()->GetTypedOuter<AActor>() ? *StructProperty->GetOwnerUField()->GetTypedOuter<AActor>()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->OwnerUObject=%s"), StructProperty->GetOwnerUObject() ? *StructProperty->GetOwnerUObject()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->OwnerUObject->Outer=%s"), StructProperty->GetOwnerUObject()->GetOuter() ? *StructProperty->GetOwnerUObject()->GetOuter()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->OwnerUObject->ActorOuter=%s"), StructProperty->GetOwnerUObject()->GetTypedOuter<AActor>() ? *StructProperty->GetOwnerUObject()->GetTypedOuter<AActor>()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->OwnerAActor=%s"), StructProperty->GetOwner<AActor>() ? *StructProperty->GetOwner<AActor>()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->OwnerClass=%s"), StructProperty->GetOwnerClass() ? *StructProperty->GetOwnerClass()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->Struct=%s"), StructProperty->Struct ? *StructProperty->Struct->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->Struct->GetOwnerClass()=%s"), StructProperty->Struct->GetOwnerClass() ? *StructProperty->Struct->GetOwnerClass()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("StructProperty->Struct->GetOwnerStruct()=%s"), StructProperty->Struct->GetOwnerStruct() ? *StructProperty->Struct->GetOwnerStruct()->GetName() : TEXT("None"));
//
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("TargetProperty=%s"), TargetProperty ? *TargetProperty->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("TargetProperty->OwnerUField=%s"), TargetProperty->GetOwnerUField() ? *TargetProperty->GetOwnerUField()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("TargetProperty->OwnerUObject=%s"), TargetProperty->GetOwnerUObject() ? *TargetProperty->GetOwnerUObject()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("TargetProperty->OwnerAActor=%s"), TargetProperty->GetOwner<AActor>() ? *TargetProperty->GetOwner<AActor>()->GetName() : TEXT("None"));
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("TargetProperty->OwnerClass=%s"), TargetProperty->GetOwnerClass() ? *TargetProperty->GetOwnerClass()->GetName() : TEXT("None"));
				//			
				//			const FIntProperty* IntProperty = CastFieldChecked<FIntProperty>(TargetProperty);
				//			const void* ExactCountValueContainer = TargetProperty->ContainerPtrToValuePtr<void>(SpawnerObject->GetClass());
				//			int32 ExactCountValue = IntProperty->GetPropertyValue(ExactCountValueContainer);
				//			UE_LOG(LogSpawnerEditor, Log, TEXT("Is ExactCount property! Value=%d"), ExactCountValue);
				//		}
				//	}*/
				//	
				//	//StructProperty->ContainerPtrToValuePtr<FSpawnCount>();
				//	//UE_LOG(LogSpawnerEditor, Log, TEXT("Value=%s"), Value ? *FString::FromInt(*Value) : TEXT("None"));
				//	
				//	//CountProperty->GetOwnerStruct()->
				//	//CountPropertyHandle->GetValue();
				//	//void* ValueAddress = CountProperty->ContainerPtrToValuePtr<void>(CountProperty);
				//	//CountPropertyHandle->GetValue(CountValue);
				//	//CountValue->Value
				//	
				//	/*float TimeValue;
				//	ActualTimePropertyHandle->GetValue(TimeValue);*/
				//	
				//	FFormatOrderedArguments Args;
				//	Args.Add(ClassNameText);
				//	Args.Add(ActualCountText);
				//	Args.Add(ActualTimeText);
				//	//Args.Add(FText::FromString(FString::Printf(TEXT("%.2f"), 1.f/*static_cast<float>(CountValue) * TimeValue)*/)));
				//	return FText::Format(LOCTEXT("SpawnListEntry_ValueDisplayText", "{0}: Count={1}, Time={2}"), Args);
				//})
				//.TextStyle(FEditorStyle::Get(), "HintText")
				//.ColorAndOpacity(FLinearColor(0.2f, 0.7f, 0.7f, 1.f))
			]
			/*SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.MaxWidth(100.f)
				[
					PropertyHandle->CreatePropertyNameWidget()
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					PropertyHandle->CreatePropertyValueWidget()
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.MaxWidth(100.f)
				[
					ClassToSpawnPropertyHandle->CreatePropertyNameWidget()
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					ClassToSpawnPropertyHandle->CreatePropertyValueWidget()
				]
			]*/
		]
	];
	/*HeaderRow.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	[
		PropertyHandle->CreatePropertyValueWidget()
	];*/

	OnClassToSpawnChanged(ClassToSpawnPropertyHandle);
	OnTimeChanged(TimePropertyHandle);
	OnCountChanged(CountPropertyHandle);
	//OnConditionalChanged(ConditionalPropertyHandle);
	OnConditionalActorsChanged(ConditionalActorsPropertyHandle);
	OnPostSpawnDataChanged(PostSpawnDataPropertyHandle);
	OnClassNameChanged(ClassNamePropertyHandle);
	OnActualCountChanged(ActualCountPropertyHandle);
	OnActualTimeChanged(ActualTimePropertyHandle);
	OnActualPostSpawnDataChanged(ActualPostSpawnDataPropertyHandle);

	ClassToSpawnPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnClassToSpawnChanged, ClassToSpawnPropertyHandle));
	TimePropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnTimeChanged, TimePropertyHandle));
	CountPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnCountChanged, CountPropertyHandle));
	//ConditionalPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnConditionalChanged, ConditionalPropertyHandle));
	ConditionalActorsPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnConditionalActorsChanged, ConditionalActorsPropertyHandle));
	PostSpawnDataPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnPostSpawnDataChanged, PostSpawnDataPropertyHandle));
	ClassNamePropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnClassNameChanged, ClassNamePropertyHandle));
	ActualCountPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnActualCountChanged, ActualCountPropertyHandle));
	ActualTimePropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnActualTimeChanged, ActualTimePropertyHandle));
	ActualPostSpawnDataPropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FSpawnListEntryCustomization::OnActualPostSpawnDataChanged, ActualPostSpawnDataPropertyHandle));
}

void FSpawnListEntryCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ChildBuilder.AddProperty(ClassToSpawnPropertyHandle.ToSharedRef());
	ChildBuilder.AddProperty(TimePropertyHandle.ToSharedRef());
	ChildBuilder.AddProperty(CountPropertyHandle.ToSharedRef());
	ChildBuilder.AddProperty(ConditionalActorsPropertyHandle.ToSharedRef());
	ChildBuilder.AddProperty(PostSpawnDataPropertyHandle.ToSharedRef());
	//ChildBuilder.AddProperty(ConditionalPropertyHandle.ToSharedRef());
	//ChildBuilder.AddProperty(ClassNamePropertyHandle.ToSharedRef());
	//ChildBuilder.AddProperty(ActualCountPropertyHandle.ToSharedRef());
	//ChildBuilder.AddProperty(ActualTimePropertyHandle.ToSharedRef());
}

void FSpawnListEntryCustomization::OnClassToSpawnChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(ClassToSpawnText);
	}
}

void FSpawnListEntryCustomization::OnTimeChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(TimeText);
	}
}

void FSpawnListEntryCustomization::OnCountChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(CountText);
	}
}

void FSpawnListEntryCustomization::OnConditionalChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(ConditionalText);
	}
}

void FSpawnListEntryCustomization::OnConditionalActorsChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(ConditionalActorsText);
	}
}

void FSpawnListEntryCustomization::OnPostSpawnDataChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(PostSpawnDataText);
	}
}

void FSpawnListEntryCustomization::OnClassNameChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(ClassNameText);
	}
}

void FSpawnListEntryCustomization::OnActualCountChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(ActualCountText);
	}
}

void FSpawnListEntryCustomization::OnActualTimeChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(ActualTimeText);
	}
}

void FSpawnListEntryCustomization::OnActualPostSpawnDataChanged(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	if (PropertyHandle.IsValid() && PropertyHandle->IsValidHandle())
	{
		PropertyHandle->GetValueAsDisplayText(ActualPostSpawnDataText);
	}
}

#undef LOCTEXT_NAMESPACE
