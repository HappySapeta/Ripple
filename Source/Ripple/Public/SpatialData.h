// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template <typename DataType>
class FRpSpatialData
{
public:
	
	FRpSpatialData()
	{
		Initialize(2, 10, {});
	}

	void Initialize(const int Resolution, const float WorldSpan, DataType Default = {})
	{
		check(Resolution > 0);

		GridSize = Resolution;
		WorldSize = WorldSpan;
		Data.Init(Default, GridSize * GridSize);
	}

	[[nodiscard]] DataType* GetDataAt(const FVector2f& Coordinates, const FVector2f& Offset = {0, 0})
	{
		const int RowIndex = static_cast<int>(FMath::Floor(Coordinates.X + Offset.X));
		const int ColumnIndex = static_cast<int>(FMath::Floor(Coordinates.Y + Offset.Y));

		if (!IsValidGridCoordinate({static_cast<float>(RowIndex), static_cast<float>(ColumnIndex)}))
		{
			return nullptr;
		}

		return &Data[RowIndex * GridSize + ColumnIndex];
	}

	[[nodiscard]] const DataType* GetDataAt(const FVector2f& Coordinates, const FVector2f& Offset = {0, 0}) const
	{
		return const_cast<FRpSpatialData*>(this)->GetDataAt(Coordinates, Offset);
	}

	float GetCellSize() const
	{
		return WorldSize / static_cast<float>(GridSize);
	}

	int GetResolution() const
	{
		return GridSize;
	}

	float GetWorldSpan() const
	{
		return WorldSize;
	}

	bool IsValidGridCoordinate(const FVector2f& Coordinate) const
	{
		return Coordinate.X >= 0 && Coordinate.X < GridSize && Coordinate.Y >= 0 && Coordinate.Y < GridSize;
	}

	bool IsValidWorldPosition(const FVector2f& Position) const
	{
		return Position.X >= 0 && Position.X <= WorldSize && Position.Y >= 0 && Position.Y <= WorldSize;
	}

	FVector2f GetCoordsOf(const int Index)
	{
		uint32 Row = Index / GetNum();
		uint32 Column = Index % GetNum();
		
		return {static_cast<float>(Row), static_cast<float>(Column)};
	}
	
	[[nodiscard]] FVector2f GridToWorld(const FVector2f& Coordinate) const
	{
		const float NormXCoordinate = Coordinate.X / static_cast<float>(GridSize);
		const float NormYCoordinate = Coordinate.Y / static_cast<float>(GridSize);

		return {NormXCoordinate * WorldSize, NormYCoordinate * WorldSize};
	}

	[[nodiscard]] FVector2f WorldToGrid(const FVector2f& Position) const
	{
		const FVector2f PreciseCoords = WorldToGridCentered(Position);
		return {FMath::Floor(PreciseCoords.X), FMath::Floor(PreciseCoords.Y)};
	}

	[[nodiscard]] FVector2f WorldToGridCentered(const FVector2f& Position) const
	{
		const float CellSize = WorldSize / GridSize;

		float X = FMath::Clamp(Position.X / CellSize, 0, GridSize - 1.0f);
		float Y = FMath::Clamp(Position.Y / CellSize, 0, GridSize - 1.0f);

		return {X, Y};
	}

	void ForEachCellPerform(TFunction<void(DataType * Cell, const FVector2f & Coords)> Operation)
	{
		for (int Row = 0; Row < GridSize; ++Row)
		{
			for (int Column = 0; Column < GridSize; ++Column)
			{
				Operation
				(
					GetDataAt({static_cast<float>(Row), static_cast<float>(Column)}), 
					FVector2f{static_cast<float>(Row), static_cast<float>(Column)}
				);
			}
		}
	}
	
	void ForEachCellPerform(TFunction<void(const DataType * Cell, const FVector2f & Coords)> Operation) const
	{
		for (int Row = 0; Row < GridSize; ++Row)
		{
			for (int Column = 0; Column < GridSize; ++Column)
			{
				Operation
				(
					GetDataAt({static_cast<float>(Row), static_cast<float>(Column)}), 
					FVector2f{static_cast<float>(Row), static_cast<float>(Column)}
				);
			}
		}
	}

	uint32 GetNum() const
	{
		return Data.Num();
	}

private:

	float WorldSize;
	int GridSize;
	TArray<DataType> Data;
};
