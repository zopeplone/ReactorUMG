#include "ReactorUMGBlueprintGeneratedClass.h"
#include "UObject/UnrealType.h"
UReactorUMGBlueprintGeneratedClass::UReactorUMGBlueprintGeneratedClass(const FObjectInitializer& ObjectInitializer)
{
	
}

void UReactorUMGBlueprintGeneratedClass::PostLoad()
{
	Super::PostLoad();
#if WITH_EDITOR
	if (!GIsEditor || IsRunningCommandlet() || IsRunningGame())
	{
		return;
	}

	for (TFieldIterator<FProperty> It(GetClass(), EFieldIteratorFlags::ExcludeSuper); It; ++It)
	{
		It->ClearValue_InContainer(this);
	}
#endif
}
