#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AzulHiloInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAzulHiloInterface : public UInterface
{
	GENERATED_BODY()
};

class AZULPROJECT_API IAzulHiloInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Hilo")
	void UpdateSpline(const FVector& NewStartPosition);
};