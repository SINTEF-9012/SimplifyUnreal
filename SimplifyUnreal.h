#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimplifyUnreal.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDINGDEV_API USimplifyUnreal : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "2D")
		static TArray<FVector2D> Simplify(TArray<FVector2D> points, float tolerance = 1.0f, bool highestQuality = false);
	
	
};
