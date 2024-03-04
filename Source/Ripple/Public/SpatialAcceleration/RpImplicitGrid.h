// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

/**
 * Data type used to store Indices.
 * Bitmasks are just binary data that indicate what objects are present in a Cell.
**/
typedef uint64 RpIndexBuffer;
constexpr int8 GIndexBufferSize = std::numeric_limits<RpIndexBuffer>::digits;

/**
* Number of IndexBuffers in a Block.
* With GIndexBufferLength of 64, and GIndexBlockSize of 20, each cell in the grid can address upto 1280 objects.
*/
constexpr int8 GIndexBlockSize = 20;

// Block of IndexBuffers
struct FRpIndexBlock
{
	FRpIndexBlock()
	{
		for(RpIndexBuffer& Buffer : IndexBuffers)
		{
			Buffer = 0;
		}
	}

	RpIndexBuffer& operator[](const uint8 Index)
	{
		return IndexBuffers[Index];
	}
	
	const RpIndexBuffer& operator[](const uint8 Index) const
	{
		return IndexBuffers[Index];
	}

	void Reset()
	{
		for(RpIndexBuffer& Buffer : IndexBuffers)
		{
			Buffer = 0;
		}
	}
	
	TStaticArray<RpIndexBuffer, GIndexBlockSize> IndexBuffers;
};

// Location of a cell in the grid represented by Row and Column Index as X and Y respectively.
struct FRpCellLocation
{
	// Row Index
	int X = 0;

	// Column Index
	int Y = 0;
};

template<typename T, uint32 N>
struct TRpStaticVector
{
	void Push(T Item)
	{
		if(Size < N)
		{
			Array[Size] = Item;
			++Size;
		}
	}

	void Pop()
	{
		if(Size > 0)
		{
			--Size;
		}
	}

	T operator[](const uint32 Index)
	{
		return Array[Index];
	}

	void Reset()
	{
		Size = 0;
	}

	uint8 Num() const
	{
		return Size;
	}
	
	TStaticArray<T, N> Array;
	
private:
	uint32 Size = 0;
};

typedef TRpStaticVector<uint32, 16> FRpSearchResults; 

/**
 * RpImplicitGrid is a Binary implementation of a Uniform, Spatial Acceleration Structure.
 * It works by mapping objects to cells in the grid, using their locations as keys.
 * It provides Search functions to find objects in a specified region quickly.
 * 
 * Each cell in the grid has two IndexBlocks for the X and Y axes.
 * IndexBlocks are collections of IndexBuffers. IndexBuffers are series of bits.
 * Each object in the Grid has a unique Index, that is ultimately defined by these BitMasks.
 */
class RIPPLE_API FRpImplicitGrid
{
public:

	FRpImplicitGrid() = default;
	
	// Reserves and initializes arrays with 0's.
	void Initialize(const FFloatRange& NewDimensions, const FPlatformTypes::uint32 NewResolution);
	
	/**
	 * 1. Iterates through all registered objects.
	 * 2. Maps their world location to a location on the Grid.
	 * 3. Updates bitmasks of the corresponding Cell.
	 */
	void Update(const TArray<FVector>& Positions);
	
	/**
	 * @brief Finds all objects in a region.
	 * @param Location Search Location
	 * @param Radius Search Radius
	 */
	void RadialSearch(const FVector& Location, const float Radius, FRpSearchResults& OutResults) const;

	void LineSearch(const FVector& StartLocation, const FVector& EndLocation, FRpSearchResults& OutResults) const;

	void DrawDebug(const UWorld* World, const float Duration) const;

protected:
	
	// Returns indices of all objects present in a certain cell.
	void GetObjectsInCell(FRpSearchResults& OutObjects) const;

	// Fills all block arrays with 0s.
	void ResetAllIndexBuffers();

	void SampleCell(const FRpCellLocation& CellLocation) const;
	
	// Converts world-space location to a location of a cell in the grid.
	FRpCellLocation TransformLocation(FVector WorldLocation) const;

	// Converts a location of a cell to a world-space location.
	FVector TransformLocation(const FRpCellLocation& GridLocation) const;

	// Checks if a given world-space location lies within the bounds of the grid. 
	bool IsValidLocation(const FVector& WorldLocation) const;

	// Checks if a given cell location is valid.
	bool IsValidLocation(const FRpCellLocation& GridLocation) const;

protected:
	
	// An array of BitBlocks that maps objects based on the X-Coordinate of their location.
	TArray<FRpIndexBlock> RowBlocks;
	
	// An array of BitBlocks that maps objects based on the Y-Coordinate of their location.
	TArray<FRpIndexBlock> ColumnBlocks;
	
	mutable FRpIndexBlock MergedRowBlocks;
	mutable FRpIndexBlock MergedColumnBlocks;

	FFloatRange Dimensions;
	uint8 Resolution = 1;

	mutable TRpStaticVector<FRpCellLocation, 200> DebugBuffer; 
	
};
