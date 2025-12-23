#pragma once
// Core includes
#include "CoreMinimal.h"

// CoreUObject includes
#include "UObject/Package.h"

// Engine includes
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Engine/BlueprintGeneratedClass.h"

#include "ReactorUMGBlueprintGeneratedClass.generated.h"

UCLASS()
class REACTORUMG_API UReactorUMGBlueprintGeneratedClass : public UWidgetBlueprintGeneratedClass
{
	GENERATED_UCLASS_BODY()

public:
	virtual void PostLoad() override;

	UPROPERTY(BlueprintReadOnly, Category="ReactorUMG")
	FString MainScriptPath;

	UPROPERTY(BlueprintReadOnly, Category="ReactorUMG")
	FString TsProjectDir;

	UPROPERTY(BlueprintReadOnly, Category="ReactorUMG")
	FString TsScriptHomeFullDir;

	UPROPERTY(BlueprintReadOnly, Category="ReactorUMG")
	FString TsScriptHomeRelativeDir;

	UPROPERTY(BlueprintReadOnly, Category="ReactorUMG")
	FString WidgetName;
};
