#include "SpawnerEditor.h"

#include "PostSpawnDataCustomization.h"
#include "SpawnConditionalActorListEntryCustomization.h"
#include "SpawnConditionalCustomization.h"
#include "SpawnCountCustomization.h"
#include "SpawnTimeCustomization.h"
#include "SpawnerObject.h"
#include "SpawnListEntryCustomization.h"

DEFINE_LOG_CATEGORY(LogSpawnerEditor);

#define LOCTEXT_NAMESPACE "FSpawnerEditorModule"

void FSpawnerEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FSpawnTime::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(FSpawnTimeCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout(FSpawnCount::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(FSpawnCountCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout(FSpawnListEntry::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(FSpawnListEntryCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout(FSpawnConditionalActorListEntry::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(FSpawnConditionalActorListEntryCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout(FSpawnConditional::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(FSpawnConditionalCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout(FPostSpawnData::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(FPostSpawnDataCustomization::MakeInstance));
	PropertyModule.NotifyCustomizationModuleChanged();
}

void FSpawnerEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSpawnTime::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSpawnCount::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSpawnListEntry::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSpawnConditionalActorListEntry::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSpawnConditional::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FPostSpawnData::StaticStruct()->GetFName());
		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSpawnerEditorModule, SpawnerEditor)