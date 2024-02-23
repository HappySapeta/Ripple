// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

/**
 * Data type used to store Indices.
 * Bitmasks are just binary data that indicate what objects are present in a Cell.
**/
typedef uint64 RpIndexBuffer;
constexpr int8 GIndexBufferLength = std::numeric_limits<RpIndexBuffer>::digits;

/**
* Number of IndexBuffers in a Block.
* With GIndexBufferLength of 64, and GIndexBlockSize of 20, each cell in the grid can address upto 1280 objects.
*/
constexpr int8 GIndexBlockSize = 20;

// Location of a cell in the grid represented by Row and Column Index as X and Y respectively.
struct FRpCellLocation
{
	// Row Index
	uint8 X = 0;

	// Column Index
	uint8 Y = 0;
};

// Wrapper over an Array of 64-bit Integers.
struct FRpIndexBlock
{
	FRpIndexBlock()
	{
		IndexBuffer.Init(0, GIndexBlockSize);
	}

	RpIndexBuffer& operator[](const uint8 Index)
	{
		return IndexBuffer[Index];
	}
	
	const RpIndexBuffer& operator[](const uint8 Index) const
	{
		return IndexBuffer[Index];
	}

	TArray<RpIndexBuffer, TInlineAllocator<GIndexBlockSize>> IndexBuffer;
};


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
	void operator ()(const FFloatRange& NewDimensions, const uint32 NewResolution);

	// Set a weak reference to array of positions of objects.
	void SetObjectLocationsArray(TWeakPtr<TArray<FVector>> In_Locations);
	
	/**
	 * 1. Iterates through all registered objects.
	 * 2. Maps their world location to a location on the Grid.
	 * 3. Updates bitmasks of the corresponding Cell.
	 */
	void Update();
	
	/**
	 * @brief Finds all objects in an area of the Grid.
	 * @param Location Search Location
	 * @param Radius Search Radius
	 */
	void Search(const FVector& Location, const float Radius) const;

	void DrawDebugGrid(const UWorld* World) const;

protected:
	
	/**
	 * @brief Returns indices of all objects present in a certain cell.
	 * @param GridLocation Location of the cell.
	 */
	void GetObjectsInCell(const FRpCellLocation& GridLocation) const;

	// Fills all block arrays with 0s.
	void ResetAllIndexBuffers();

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

	// Array containing Locations of objects.
	TWeakPtr<TArray<FVector>> Locations;

private:
	
	FFloatRange Dimensions;
	uint8 Resolution = 1;
	
};
