// Copyright Anupam Sahu. All Rights Reserved.

#include "SpatialAcceleration/RpImplicitGrid.h"

void FRpImplicitGrid::operator()(const FFloatRange& NewDimensions, const uint32 NewResolution)
{
	if(ensureMsgf(NewResolution > 0, TEXT("Resolution cannot be zero.")))
	{
		Dimensions = NewDimensions;
		Resolution = NewResolution;
		
		RowBlocks.Init(FRpBitBlock(), NewResolution);
		ColumnBlocks.Init(FRpBitBlock(), NewResolution);
	}
}

void FRpImplicitGrid::SetPositionsArray(TWeakPtr<TArray<FVector>> PositionsArray)
{
	Positions = PositionsArray;
}

void FRpImplicitGrid::Search(const FVector& Location, const float Radius, FRpGridSearchResult& Out_SearchResults, uint32& Out_NumResults) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FRpImplicitGrid::Search)
	
	const int Reach = FMath::FloorToInt(Radius / Dimensions.Size<float>() * Resolution);

	FRpCellLocation SearchGridLocation;
	if(!ConvertWorldToGridLocation(Location, SearchGridLocation))
	{
		return;
	}
	
	const FRpCellLocation& StartGridLocation = FRpCellLocation(SearchGridLocation.X - Reach, SearchGridLocation.Y - Reach);
	const FRpCellLocation& EndGridLocation = FRpCellLocation(SearchGridLocation.X + Reach, SearchGridLocation.Y + Reach);
	
	Out_NumResults = 0;
	
	FRpCellLocation CurrentGridLocation = StartGridLocation;
	while(CurrentGridLocation.X <= EndGridLocation.X)
	{
		while(CurrentGridLocation.Y <= EndGridLocation.Y)
		{
			if(IsValidGridLocation(CurrentGridLocation))
			{
				GetObjectsInCell(CurrentGridLocation, Out_SearchResults, Out_NumResults);
				if(static_cast<int32>(Out_NumResults) >= Out_SearchResults.Num())
				{
					return;
				}
			}
			CurrentGridLocation.Y += 1;
		}
		CurrentGridLocation.Y = StartGridLocation.Y;
		CurrentGridLocation.X += 1;
	}
}

void FRpImplicitGrid::Update()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FRpImplicitGrid::Update)
	
	ResetBlocks();

	if(!Positions.IsValid())
	{
		return;
	}

	uint32 NumObjects = Positions.Pin()->Num();
	for(uint32 Index = 0; Index < NumObjects; ++Index)
	{
		if(Index >= GBitBlockSize * GBitMaskSize) break;
		
		// Find array indices
		FRpCellLocation GridLocation;
		if(!ConvertWorldToGridLocation(Positions.Pin()->operator[](Index), GridLocation))
		{
			continue;
		}
		
		// Create AdditiveMask
		const uint32 BlockLevel = Index / GBitMaskSize;
		const uint32 BitLocation = Index % GBitMaskSize;
		const BitMaskType AdditiveMask = static_cast<BitMaskType>(1) << BitLocation;
		
		// Apply AdditiveMask
		RowBlocks[GridLocation.X][BlockLevel] |= AdditiveMask;
		ColumnBlocks[GridLocation.Y][BlockLevel] |= AdditiveMask;
	}
}

void FRpImplicitGrid::GetObjectsInCell(const FRpCellLocation& GridLocation, FRpGridSearchResult& Out_Indices, uint32& Out_NumIndices) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(USpatialGridSubsystem::GetIndicesInGridLocation)
	if(static_cast<int32>(Out_NumIndices) >= Out_Indices.Num() || !IsValidGridLocation(GridLocation))
	{
		return;
	}

	uint32 Index = Out_NumIndices;
	for(uint32 BlockLevel = 0; BlockLevel < GBitBlockSize; ++BlockLevel)
	{
		const BitMaskType IndicesInThisBlock = RowBlocks[GridLocation.X][BlockLevel] & ColumnBlocks[GridLocation.Y][BlockLevel];

		for(int BitLocation = 0; BitLocation < GBitMaskSize; ++BitLocation)
		{
			const BitMaskType FilteredBlock = IndicesInThisBlock & (static_cast<BitMaskType>(1) << BitLocation);
			if(FilteredBlock != 0)
			{
				Out_Indices[Index] = BlockLevel * GBitMaskSize + BitLocation;
				++Index;
				++Out_NumIndices;

				if(static_cast<int32>(Out_NumIndices) >= Out_Indices.Num())
				{
					return;
				}
			}
		}
	}
}

bool FRpImplicitGrid::ConvertWorldToGridLocation(FVector WorldLocation, FRpCellLocation& Out_GridLocation) const
{
	if(!IsValidWorldLocation(WorldLocation))
	{
		return false;
	}

	const float CellSize = Dimensions.Size<float>() / Resolution;

	WorldLocation.X -= Dimensions.GetLowerBoundValue();
	WorldLocation.Y -= Dimensions.GetLowerBoundValue();

	Out_GridLocation.X = FMath::Clamp(WorldLocation.X / CellSize, 0, Resolution - 1);
	Out_GridLocation.Y = FMath::Clamp(WorldLocation.Y / CellSize, 0, Resolution - 1);
	
	return true;
}

bool FRpImplicitGrid::ConvertGridToWorldLocation(const FRpCellLocation& GridLocation, FVector& Out_WorldLocation) const
{
	if(!IsValidGridLocation(GridLocation))
	{
		return false;
	}

	const float RangeSize = Dimensions.Size<float>();
	
	// Normalize
	Out_WorldLocation.X = GridLocation.X / static_cast<float>(Resolution);
	Out_WorldLocation.Y = GridLocation.Y / static_cast<float>(Resolution);

	// Scale
	Out_WorldLocation.X *= RangeSize;
	Out_WorldLocation.Y *= RangeSize;

	// Translate
	const float LowerBoundValue = Dimensions.GetLowerBoundValue();
	const float CellSize = RangeSize / Resolution;
	Out_WorldLocation.X += LowerBoundValue + (CellSize * 0.5f);
	Out_WorldLocation.Y += LowerBoundValue + (CellSize * 0.5f);
	
	return true;
}

bool FRpImplicitGrid::IsValidWorldLocation(const FVector& WorldLocation) const
{
	return Dimensions.Contains(WorldLocation.X) && Dimensions.Contains(WorldLocation.Y);
}

bool FRpImplicitGrid::IsValidGridLocation(const FRpCellLocation& GridLocation) const
{
	return RowBlocks.IsValidIndex(GridLocation.X) && ColumnBlocks.IsValidIndex(GridLocation.Y);
}

void FRpImplicitGrid::ResetBlocks()
{
	for(uint32 BlockIndex = 0; BlockIndex < Resolution; ++BlockIndex)
	{
		for(int BlockLevel = 0; BlockLevel < GBitBlockSize; ++BlockLevel)
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
