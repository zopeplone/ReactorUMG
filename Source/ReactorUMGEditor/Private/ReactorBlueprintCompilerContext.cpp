#include "ReactorBlueprintCompilerContext.h"

#include "ReactorUMGWidgetBlueprint.h"
#include "ReactorUMGBlueprintGeneratedClass.h"
#include "ReactorUMGUtilityWidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet2/KismetReinstanceUtilities.h"

FReactorUMGBlueprintCompilerContext::FReactorUMGBlueprintCompilerContext(UWidgetBlueprint *SourceBlueprint,
																		 FCompilerResultsLog &InMessageLog, const FKismetCompilerOptions &InCompilerOptions)
	: Super(SourceBlueprint, InMessageLog, InCompilerOptions)
{
}

FReactorUMGBlueprintCompilerContext::~FReactorUMGBlueprintCompilerContext()
{
}

void FReactorUMGBlueprintCompilerContext::EnsureProperGeneratedClass(UClass *&TargetUClass)
{
	if (TargetUClass && !((UObject *)TargetUClass)->IsA(UReactorUMGBlueprintGeneratedClass::StaticClass()))
	{
		FKismetCompilerUtilities::ConsignToOblivion(TargetUClass, Blueprint->bIsRegeneratingOnLoad);
		TargetUClass = nullptr;
	}
}

void FReactorUMGBlueprintCompilerContext::SpawnNewClass(const FString &NewClassName)
{
	UReactorUMGBlueprintGeneratedClass *BlueprintGeneratedClass = FindObject<UReactorUMGBlueprintGeneratedClass>(Blueprint->GetOutermost(), *NewClassName);

	if (BlueprintGeneratedClass == nullptr)
	{
		BlueprintGeneratedClass = NewObject<UReactorUMGBlueprintGeneratedClass>(Blueprint->GetOutermost(), FName(*NewClassName), RF_Public | RF_Transactional);
	}
	else
	{
		FBlueprintCompileReinstancer::Create(BlueprintGeneratedClass);
	}

	NewClass = BlueprintGeneratedClass;
}

void FReactorUMGBlueprintCompilerContext::CopyTermDefaultsToDefaultObject(UObject *DefaultObject)
{
}

void FReactorUMGBlueprintCompilerContext::FinishCompilingClass(UClass *Class)
{
	if (Class)
	{
		UE_LOG(LogTemp, Log, TEXT("FinishCompilingClass %s"), *Class->GetName())
	}

	if (UReactorUMGWidgetBlueprint *WidgetBlueprint = Cast<UReactorUMGWidgetBlueprint>(Blueprint))
	{
		// 在执行 JS 脚本之前，清理 WidgetTree 中之前编译时创建的 Widget
		// 确保从干净的状态开始，每次编译都重新创建所有 Widget
		if (WidgetBlueprint->WidgetTree)
		{
			TArray<UWidget *> AllWidgets;
			WidgetBlueprint->WidgetTree->GetAllWidgets(AllWidgets);
			for (UWidget *Widget : AllWidgets)
			{
				if (Widget)
				{
					Widget->MarkAsGarbage();
				}
			}
		}
		WidgetBlueprint->WidgetVariableNameToGuidMap.Empty();

		const FReactorUMGCompilerLog Logger(MessageLog);
		WidgetBlueprint->SetupTsScripts(Logger, true, true);
		UReactorUMGBlueprintGeneratedClass *BPGClass = CastChecked<UReactorUMGBlueprintGeneratedClass>(Class);
		if (BPGClass)
		{
			BPGClass->MainScriptPath = WidgetBlueprint->GetMainScriptPath();
			BPGClass->TsProjectDir = WidgetBlueprint->GetTsProjectDir();
			BPGClass->TsScriptHomeFullDir = WidgetBlueprint->GetTsScriptHomeFullDir();
			BPGClass->TsScriptHomeRelativeDir = WidgetBlueprint->GetTsScriptHomeRelativeDir();
			BPGClass->WidgetName = WidgetBlueprint->GetWidgetName();
		}
	}

	if (UReactorUMGUtilityWidgetBlueprint *UtilityWidgetBlueprint = Cast<UReactorUMGUtilityWidgetBlueprint>(Blueprint))
	{
		// 在执行 JS 脚本之前，清理 WidgetTree 中之前编译时创建的 Widget
		// 确保从干净的状态开始，每次编译都重新创建所有 Widget
		if (UtilityWidgetBlueprint->WidgetTree)
		{
			TArray<UWidget *> AllWidgets;
			UtilityWidgetBlueprint->WidgetTree->GetAllWidgets(AllWidgets);
			for (UWidget *Widget : AllWidgets)
			{
				if (Widget)
				{
					Widget->MarkAsGarbage();
				}
			}
		}
		UtilityWidgetBlueprint->WidgetVariableNameToGuidMap.Empty();

		const FReactorUMGCompilerLog Logger(MessageLog);
		UtilityWidgetBlueprint->SetupTsScripts(Logger, true, true);
		UReactorUMGBlueprintGeneratedClass *BPGClass = CastChecked<UReactorUMGBlueprintGeneratedClass>(Class);
		if (BPGClass)
		{
			BPGClass->MainScriptPath = UtilityWidgetBlueprint->GetMainScriptPath();
			BPGClass->TsProjectDir = UtilityWidgetBlueprint->GetTsProjectDir();
			BPGClass->TsScriptHomeFullDir = UtilityWidgetBlueprint->GetTsScriptHomeFullDir();
			BPGClass->TsScriptHomeRelativeDir = UtilityWidgetBlueprint->GetTsScriptHomeRelativeDir();
			BPGClass->WidgetName = UtilityWidgetBlueprint->GetWidgetName();
		}
	}

	Super::FinishCompilingClass(Class);
}