#pragma once

#include "GridGenerator.generated.h"

UCLASS(NotBlueprintable)
class RIPPLELIBRARY_API URPGridGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, DisplayName = "2D Grid Generator")
	static TArray<FVector2D> Generate2D(const FVector2D& Origin, const FVector2D& Span, float Separation);

	UFUNCTION(BlueprintCallable, DisplayName = "3D Grid Generator")
	static TArray<FVector> Generate3D(const FVector& Origin, const FVector& Span, float Separation);
};
