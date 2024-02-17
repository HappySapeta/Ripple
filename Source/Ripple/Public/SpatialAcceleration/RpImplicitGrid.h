// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

// Represents a location on the grid in terms of Column and Row Indices.
struct FRpCellLocation
{
	FRpCellLocation(const int32 XIndex = 0, const int32 YIndex = 0)
		: X(XIndex), Y(YIndex)
	{}
	
	int32 X; // Row Index
	int32 Y; // Column Index
};

/**
 * Data type used to store Indices.
 * Bitmasks are just binary data that indicate what objects are present in a Cell.
**/
typedef uint64 BitMaskType;
constexpr int8 GBitMaskSize = std::numeric_limits<BitMaskType>::digits;

/**
* Number of bitmasks in a Block.
* E.g : with GBitRowSize of 64 (for RowType uint64), and GBlockSize of 20, each cell of the grid can address 20 * 64 = 1280 objects.
*/
constexpr int8 GBitBlockSize = 20;

// Defines the maximum number of objects that can be found during a search.
constexpr int MAX_SEARCH_RESULTS = 5;

// Static array of indices of objects found during a search.
typedef TStaticArray<uint16, MAX_SEARCH_RESULTS> FRpGridSearchResult;

// Wrapper over an Array of 64-bit Integers.
struct FRpBitBlock
{
	FRpBitBlock()
	{
		BitRow.Init(0, GBitBlockSize);
	}

	BitMaskType& operator[](const uint32 Index)
	{
		return BitRow[Index];
	}
	
	const BitMaskType& operator[](const uint32 Index) const
	{
		return BitRow[Index];
	}

	TArray<BitMaskType, TInlineAllocator<GBitBlockSize>> BitRow;
};


/**
 * RpImplicitGrid implements a Uniform, Binary, Spatial Acceleration Structure
 * called an Implicit Grid. It works by mapping objects to cells in the grid,
 * using their locations as keys. It provides a fast lookup to find objects in a specified region.
 * 
 * Each cell in the grid has two BitBlocks for the X and Y axes.
 * BitBlocks are collection of BitMasks. BitMasks are just a collection of Bits.
 * Each object in the Grid has a unique Index, that is ultimately defined by these BitMasks.
 */
class RIPPLE_API FRpImplicitGrid
{
public:

	// Reserves and initializes arrays with 0's.
	void operator ()(const FFloatRange& NewDimensions, const uint32 NewResolution);
	
	FRpImplicitGrid() = default;

	// Set a weak reference to array of positions of objects.
	void SetPositionsArray(TWeakPtr<TArray<FVector>> PositionsArray);
	
	/**
	 * 1. Iterates through all registered objects.
	 * 2. Maps their world location to a location on the Grid.
	 * 3. Updates bitmasks of the corresponding Cell.
	 */
	virtual void Update();
	
	/**
	 * @brief Finds all objects in an area of the Grid.
	 * @param Location Search Location
	 * @param Radius Search Radius
	 * @param Out_SearchResults Vector of Indices of objects found in the search.
	 * @param Out_NumResults Number of objects found. This may not be equal to the length of Out_SearchResults. 
	 */
	virtual void Search(const FVector& Location, const float Radius, FRpGridSearchResult& Out_SearchResults, uint32& Out_NumResults) const;

	void DrawDebugGrid(const UWorld* World) const;

	virtual ~FRpImplicitGrid() = default;

protected:
	
	/**
	 * @brief Returns indices of all objects present in a certain cell.
	 * @param GridLocation Location of the cell.
	 * @param Out_Indices Vector of Indices of objects found in the cell.
	 * @param Out_NumIndices Number of objects found. This may not be equal to the length of Out_Indices. 
	 */
	void GetObjectsInCell(const FRpCellLocation& GridLocation, FRpGridSearchResult& Out_Indices, uint32& Out_NumIndices) const;

	// Fills all block arrays with 0s.
	virtual void ResetBlocks();

	// Converts world-space location to a location of a cell in the grid.
	bool ConvertWorldToGridLocation(FVector WorldLocation, FRpCellLocation& Out_GridLocation) const;

	// Converts a location of a cell to a world-space location.
	bool ConvertGridToWorldLocation(const FRpCellLocation& GridLocation, FVector& Out_WorldLocation) const;

	// Checks if a given world-space location lies within the bounds of the grid. 
	bool IsValidWorldLocation(const FVector& WorldLocation) const;

	// Checks if a given cell location is valid.
	bool IsValidGridLocation(const FRpCellLocation& GridLocation) const;

protected:
	
	// An array of BitBlocks that maps objects based on the X-Coordinate of their location.
	TArray<FRpBitBlock> RowBlocks;
	
	// An array of BitBlocks that maps objects based on the Y-Coordinate of their location.
	TArray<FRpBitBlock> ColumnBlocks;

	// Array containing Locations of objects.
	TWeakPtr<TArray<FVector>> Positions;

private:
	
	FFloatRange Dimensions;
	uint32 Resolution = 1;
	
};
