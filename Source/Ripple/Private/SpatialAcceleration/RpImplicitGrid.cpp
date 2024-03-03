// Copyright Anupam Sahu. All Rights Reserved.

#include "SpatialAcceleration/RpImplicitGrid.h"

void FRpImplicitGrid::Initialize
(
	const FFloatRange& NewDimensions,
	const uint32 NewResolution,
	const TWeakPtr<TArray<FVector>>& InLocations
)
{
	check(NewResolution > 0);
	Dimensions = NewDimensions;
	Resolution = NewResolution;
	
	RowBlocks.Init(FRpIndexBlock(), NewResolution);
	ColumnBlocks.Init(FRpIndexBlock(), NewResolution);

	check(InLocations.IsValid());
	Locations = InLocations;
}

void FRpImplicitGrid::RadialSearch
(
	const FVector& Location,
	const float Radius,
	FRpSearchResults& OutResults
) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UTrSimulationSystem::RadialSearch)

	MergedRowBlocks.Reset();
	MergedColumnBlocks.Reset();

	const FRpCellLocation LowerBound = TransformLocation(Location - Radius * FVector::One());
	const FRpCellLocation UpperBound = TransformLocation(Location + Radius * FVector::One());
	
	for (uint8 X = LowerBound.X; X <= UpperBound.X; ++X)
	{
		for (uint8 BufferIndex = 0; BufferIndex < GIndexBlockSize; ++BufferIndex)
		{
			MergedRowBlocks[BufferIndex] |= RowBlocks[X][BufferIndex];
		}
	}

	for (uint8 Y = LowerBound.Y; Y <= UpperBound.Y; ++Y)
	{
		for (uint8 BufferIndex = 0; BufferIndex < GIndexBlockSize; ++BufferIndex)
		{
			MergedColumnBlocks[BufferIndex] |= ColumnBlocks[Y][BufferIndex];
		}
	}

	GetObjectsInCell(OutResults);
}

void FRpImplicitGrid::LineSearch
(
	const FVector& StartLocation,
	const FVector& EndLocation,
	FRpSearchResults& OutResults
) const
{
	if(!IsValidLocation(StartLocation))
	{
		return;
	}

	MergedRowBlocks.Reset();
	MergedColumnBlocks.Reset();
	
	const float X1 = StartLocation.X;
	const float Y1 = StartLocation.Y;
	const float X2 = EndLocation.X;
	const float Y2 = EndLocation.Y;
	
	auto [I, J] = TransformLocation(StartLocation);
	const auto [IEnd, JEnd] = TransformLocation(EndLocation);

	const int DeltaI = ((X1 < X2) ? 1 : ((X1 > X2) ? -1 : 0));
	const int DeltaJ = ((Y1 < Y2) ? 1 : ((Y1 > Y2) ? -1 : 0));

	const float CellSize = Dimensions.Size<float>() / Resolution;
	const float MinX = CellSize * FMath::Floor(X1 / CellSize);
	const float MaxX = MinX + CellSize;
	const float MinY = CellSize * FMath::Floor(Y1 / CellSize);
	const float MaxY = MinY + CellSize;

	float Tx = ((X1 < X2) ? (X1 - MinX) : (MaxX - X1)) / FMath::Abs(X2 - X1);
	float Ty = ((Y1 < Y2) ? (Y1 - MinY) : (MaxY - Y1)) / FMath::Abs(Y2 - Y1);

	const float DeltaTx = CellSize / FMath::Abs(X2 - X1);
	const float DeltaTy = CellSize / FMath::Abs(Y2 - Y1);

	while(true)
	{
		if(IsValidLocation({I, J}))
		{
			DebugBuffer.Push({I, J});
			for (uint8 BufferIndex = 0; BufferIndex < GIndexBlockSize; ++BufferIndex)
			{
				MergedRowBlocks[BufferIndex] |= RowBlocks[I][BufferIndex];
			}
			
			for (uint8 BufferIndex = 0; BufferIndex < GIndexBlockSize; ++BufferIndex)
			{
				MergedColumnBlocks[BufferIndex] |= ColumnBlocks[J][BufferIndex];
			}
		}
		else
		{
			break;
		}
		
		if(Tx <= Ty)
		{
			if(I == IEnd)
			{
				break;
			}
			Tx += DeltaTx;
			I += DeltaI;
		}
		else
		{
			if(J == JEnd)
			{
				break;
			}
			Ty += DeltaTy;
			J += DeltaJ;
		}
	}

	GetObjectsInCell(OutResults);
}

void FRpImplicitGrid::GetObjectsInCell(FRpSearchResults& OutObjects) const
{
	for (uint8 BufferId = 0; BufferId < GIndexBlockSize; ++BufferId)
	{
		RpIndexBuffer Buffer = MergedRowBlocks[BufferId] & MergedColumnBlocks[BufferId];
		while (Buffer)
		{
			const RpIndexBuffer Object = Buffer & ~(Buffer - 1);
			const uint16 ObjectIndex = static_cast<uint16>(log2(Object)) + BufferId * GIndexBufferSize;
			OutObjects.Push(ObjectIndex);
			Buffer ^= Object;
		}
	}
}

void FRpImplicitGrid::Update()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(FRpImplicitGrid::Update)
	
	if(!Locations.IsValid())
	{
		return;
	}
	
	ResetAllIndexBuffers();

	const uint32 NumObjects = Locations.Pin()->Num();
	if(ensureMsgf(NumObjects < GIndexBlockSize * GIndexBufferSize, TEXT("Number of objects assigned to the grid exceeds its capacity.")))
	{
		for (uint32 Index = 0; Index < NumObjects; ++Index)
		{
			const FVector& ObjectLocation = Locations.Pin()->operator[](Index);

			// Find array indices
			if (!IsValidLocation(ObjectLocation))
			{
				continue;
			}

			// Create AdditiveMask
			const uint32 BlockLevel = Index / GIndexBufferSize;
			const uint32 BitLocation = Index % GIndexBufferSize;
			const RpIndexBuffer AdditiveMask = static_cast<RpIndexBuffer>(1) << BitLocation;

			// Apply AdditiveMask
			const FRpCellLocation GridLocation = TransformLocation(ObjectLocation);
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
		FMath::Clamp<uint8>(FMath::FloorToInt(WorldLocation.X / CellSize), 0, Resolution - 1),
		FMath::Clamp<uint8>(FMath::FloorToInt(WorldLocation.Y / CellSize), 0, Resolution - 1)
	};
}

FVector FRpImplicitGrid::TransformLocation(const FRpCellLocation& GridLocation) const
{
	const float RangeSize = Dimensions.Size<float>();

	// Normalize
	float X = GridLocation.X / static_cast<float>(Resolution);
	float Y = GridLocation.Y / static_cast<float>(Resolution);

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
	for (uint32 BlockIndex = 0; BlockIndex < Resolution; ++BlockIndex)
	{
		for (int BlockLevel = 0; BlockLevel < GIndexBlockSize; ++BlockLevel)
		{
			RowBlocks[BlockIndex][BlockLevel] = 0;
			ColumnBlocks[BlockIndex][BlockLevel] = 0;
		}
	}
}

void FRpImplicitGrid::DrawDebug(const UWorld* World, const float Duration) const
{
	const float CellWidth = Dimensions.Size<float>() / Resolution;

	float VariableCoordinate = Dimensions.GetLowerBoundValue();
	while (VariableCoordinate <= Dimensions.GetUpperBoundValue())
	{
		FVector LineStart = FVector(Dimensions.GetLowerBoundValue(), VariableCoordinate, 0.0f);
		FVector LineEnd = FVector(Dimensions.GetUpperBoundValue(), VariableCoordinate, 0.0f);
		DrawDebugLine(World, LineStart, LineEnd, FColor::White, false, Duration);

		LineStart = FVector(VariableCoordinate, Dimensions.GetLowerBoundValue(), 0.0f);
		LineEnd = FVector(VariableCoordinate, Dimensions.GetUpperBoundValue(), 0.0f);
		DrawDebugLine(World, LineStart, LineEnd, FColor::White, false, Duration);

		VariableCoordinate += CellWidth;
	}

	constexpr float CELL_DEBUG_SCALE = 0.25f;
	const float CellSize = Dimensions.Size<float>() / Resolution;
	for(const FRpCellLocation& CellLocation : DebugBuffer.Array)
	{
		FVector Extents = FVector(CellSize) * CELL_DEBUG_SCALE;
		DrawDebugBox(World, TransformLocation(CellLocation), Extents, FColor::Green, false, Duration);
	}

	DebugBuffer.Reset();
}
