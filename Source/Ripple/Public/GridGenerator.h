#pragma once

#include "GridGenerator.generated.h"

UCLASS(NotBlueprintable)
class RIPPLE_API URPGridGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable, DisplayName = "2D Grid Generator")
	static TArray<FVector2D> Generate2D(const FVector2D& Origin, const FVector2D& Span, float Separation)
	{
		if(FMath::IsNearlyZero(Separation, 0.001f))
		{
			return {};
		}
	
		TArray<FVector2D> Points;
	
		const FVector2D MinWorldLocation = Origin - Span * 0.5f;

		const uint32 NumRows = Span.X / Separation;
		const uint32 NumCols = Span.Y / Separation;
	
		FVector2D Location = MinWorldLocation;

		for (uint32 Row = 1; Row <= NumRows; ++Row)
		{
			for (uint32 Column = 1; Column <= NumCols; ++Column)
			{
				Points.Add(Location + Separation * 0.5f);
				Location.Y += Separation;
			}

			Location.X += Separation;
			Location.Y = MinWorldLocation.Y;
		}

		return Points;
	}

	UFUNCTION(BlueprintCallable, DisplayName = "3D Grid Generator")
	static TArray<FVector> Generate3D(const FVector& Origin, const FVector& Span, float Separation)
	{
		if(FMath::IsNearlyZero(Separation, 0.001f))
		{
			return {};
		}
	
		TArray<FVector> Points;
	
		const FVector MinWorldLocation = Origin - Span * 0.5f;

		const uint32 NumRows = Span.X / Separation;
		const uint32 NumCols = Span.Y / Separation;
		const uint32 NumStacks = Span.Z / Separation;
	
		FVector Location = MinWorldLocation;

		for (uint32 Row = 1; Row <= NumRows; ++Row)
		{
			for (uint32 Column = 1; Column <= NumCols; ++Column)
			{
				for(uint32 Stack = 1; Stack <= NumStacks; ++Stack)
				{
					Points.Add(Location + Separation * 0.5f);
					Location.Z += Separation;
				}
				Location.Y += Separation;
				Location.Z = MinWorldLocation.Z;
			}

			Location.X += Separation;
			Location.Y = MinWorldLocation.Y;
			Location.Z = MinWorldLocation.Z;
		}

		return Points;
	}
};
