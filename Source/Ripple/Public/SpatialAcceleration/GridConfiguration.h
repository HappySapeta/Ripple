#pragma once

#include <CoreMinimal.h>
#include "GridConfiguration.generated.h"

/**
 * Configures a Spatial Grid's properties like dimensions and resolution - including debugging options.
 */
UCLASS(Blueprintable)
class UGridConfiguration : public UDataAsset
{
	GENERATED_BODY()

public:

	// Number of rows and columns in the Grid.
	UPROPERTY(EditDefaultsOnly, Category = "Grid Setup", meta = (UIMin = "5", UIMax = "100", ClampMin = "1", ClampMax = "100"))
	uint32 Resolution = 10;

	// World space range of the Spatial Grid along X & Y Dimensions.
	UPROPERTY(EditDefaultsOnly, Category = "Grid Setup")
	FFloatRange Range = FFloatRange(-2000.0f, 2000.0f);

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "Debug Settings")
	bool bDrawGrid = false;

	UPROPERTY(EditDefaultsOnly, Category = "Debug Settings", meta = (EditCondition = "bDrawGrid", EditConditionHides = "true"))
	FColor GridColor = FColor::Green;

#pragma region BUGGYCODE
	//UPROPERTY(EditDefaultsOnly, Category = "Debug Settings")
	bool bDebugLookup = false;

	//UPROPERTY(EditDefaultsOnly, Category = "Debug Settings", meta = (EditCondition = "bDebugLookup", EditConditionHides = "true"))
	FColor LookupBoxColor = FColor::Red;
	
	//UPROPERTY(EditDefaultsOnly, Category = "Debug Settings", meta = (EditCondition = "bDebugLookup", EditConditionHides = "true"))
	float LookupBoxSize = 10.0f;
#pragma endregion
	
};
