// Copyright Anupam Sahu. All Rights Reserved.

#include "SpatialAcceleration/RpImplicitGrid.h"

void FRpImplicitGrid::operator()(const FFloatRange& NewDimensions, const uint32 NewResolution)
{
	if(ensureMsgf(NewResolution > 0, TEXT("Resolution cannot be zero.")))
	{
		Dimensions = NewDimensions;
		Resolution = NewResolution;
		
		RowBlocks.Init(FRpIndexBlock(), NewResolution);
		ColumnBlocks.Init(FRpIndexBlock(), NewResolution);
	}
}

void FRpImplicitGrid::SetObjectLocationsArray(TWeakPtr<TArray<FVector>> In_Locations)
{
	Locations = In_Locations;
}

void FRpImplicitGrid::RadialSearch(const FVector& Location, const float Radius, FRpSearchResults& Out_Results) const
{
	// todo : implement RadialSearch function
}

void FRpImplicitGrid::LinearSearch(const FVector& StartLocation, const FVector& EndLocation, const float Radius, FRpSearchResults& Out_Results) const
{
	// todo : implement LinearSearch function
}

void FRpImplicitGrid::GetObjectsInCell(const FRpCellLocation& CellLocation, FRpSearchResults& Out_Results) const
{
	// todo : rewrite GetObjectsInCell function
}

void FRpImplicitGrid::Update()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FRpImplicitGrid::Update)
	
	ResetAllIndexBuffers();

	if(!Locations.IsValid())
	{
		return;
	}

	uint32 NumObjects = Locations.Pin()->Num();
	if(ensureMsgf(NumObjects < GIndexBlockSize * GIndexBufferLength, TEXT("Number of objects assigned to the grid exceeds its capacity.")))
	{
		for(uint32 Index = 0; Index < NumObjects; ++Index)
		{
			const FVector& ObjectLocation = Locations.Pin()->operator[](Index);
		
			// Find array indices
			if(!IsValidLocation(ObjectLocation))
			{
				continue;
			}

			FRpCellLocation GridLocation = TransformLocation(ObjectLocation);
		
			// Create AdditiveMask
			const uint32 BlockLevel = Index / GIndexBufferLength;
			const uint32 BitLocation = Index % GIndexBufferLength;
			const RpIndexBuffer AdditiveMask = static_cast<RpIndexBuffer>(1) << BitLocation;
		
			// Apply AdditiveMask
			RowBlocks[GridLocation.X][BlockLevel] |= AdditiveMask;
			ColumnBlocks[GridLocation.Y][BlockLevel] |= AdditiveMask;
		}
	}
}

FRpCellLocation FRpImplicitGrid::TransformLocation(FVector WorldLocation) const
{
	const float CellSize = Dimensions.Size<float>() / Resolution;

	WorldLocation.X -= Dimensions.GetLowerBoundValue();
	WorldLocation.Y -= Dimensions.GetLowerBoundValue();

	return FRpCellLocation
	{
		FMath::Clamp<uint8>(WorldLocation.X / CellSize, 0, Resolution - 1),
		FMath::Clamp<uint8>(WorldLocation.Y / CellSize, 0, Resolution - 1)
	};
}

FVector FRpImplicitGrid::TransformLocation(const FRpCellLocation& GridLocation) const
{
	const float RangeSize = Dimensions.Size<float>();

	float X, Y;
	
	// Normalize
	X = GridLocation.X / static_cast<float>(Resolution);
	Y = GridLocation.Y / static_cast<float>(Resolution);

	// Scale
	X *= RangeSize;
	Y *= RangeSize;

	// Translate
	const float LowerBoundValue = Dimensions.GetLowerBoundValue();
	const float CellSize = RangeSize / Resolution;
	X += LowerBoundValue + (CellSize * 0.5f);
	Y += LowerBoundValue + (CellSize * 0.5f);

	return {X, Y, 0};
}

bool FRpImplicitGrid::IsValidLocation(const FVector& WorldLocation) const
{
	return Dimensions.Contains(WorldLocation.X) && Dimensions.Contains(WorldLocation.Y);
}

bool FRpImplicitGrid::IsValidLocation(const FRpCellLocation& GridLocation) const
{
	return RowBlocks.IsValidIndex(GridLocation.X) && ColumnBlocks.IsValidIndex(GridLocation.Y);
}

void FRpImplicitGrid::ResetAllIndexBuffers()
{
	for(uint32 BlockIndex = 0; BlockIndex < Resolution; ++BlockIndex)
	{
		for(int BlockLevel = 0; BlockLevel < GIndexBlockSize; ++BlockLevel)
		{
			RowBlocks[BlockIndex][BlockLevel] = 0;
			ColumnBlocks[BlockIndex][BlockLevel] = 0;
		}
	}
}

void FRpImplicitGrid::DrawDebugGrid(const UWorld* World) const
{
	const float CellWidth = Dimensions.Size<float>() / Resolution;

	float VariableCoordinate = Dimensions.GetLowerBoundValue();
	while(VariableCoordinate <= Dimensions.GetUpperBoundValue())
	{
		FVector LineStart = FVector(Dimensions.GetLowerBoundValue(), VariableCoordinate, 0.0f);
		FVector LineEnd = FVector(Dimensions.GetUpperBoundValue(), VariableCoordinate, 0.0f);

		DrawDebugLine(World, LineStart, LineEnd, FColor::White, true);

		LineStart = FVector(VariableCoordinate, Dimensions.GetLowerBoundValue(), 0.0f);
		LineEnd = FVector(VariableCoordinate, Dimensions.GetUpperBoundValue(), 0.0f);

		DrawDebugLine(World, LineStart, LineEnd, FColor::White, true);

		VariableCoordinate += CellWidth;
	}
}
