// Copyright Anupam Sahu. All Rights Reserved.

#include "RpSpatialGraphComponent.h"

URpSpatialGraphComponent::URpSpatialGraphComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URpSpatialGraphComponent::CreateGraph()
{
	if(Nodes.Num() == 0)
	{
		// Create at least one node to make the graph editable
		AddNode(FVector(0, 0, 100));
	}
}

void URpSpatialGraphComponent::DeleteGraph()
{
	Nodes.Reset();
}

const TArray<FRpSpatialGraphNode>* URpSpatialGraphComponent::GetNodes() const
{
	return &Nodes;
}

int32 URpSpatialGraphComponent::AddNode(const FVector& Location)
{
	const int32 NewNodeIndex = Nodes.Add(FRpSpatialGraphNode());
	SetNodeLocation(NewNodeIndex, Location);
	
	return NewNodeIndex;
}

void URpSpatialGraphComponent::DeleteNode(const int32 Index)
{
	// Remove all connections of the affected node
	for(uint32 ConnectedIndex : Nodes[Index].Connections)
	{
		Nodes[ConnectedIndex].Connections.Remove(Index);
	}

	// Update the connections of the other affected node that is ultimately swapped with the deleted node
	uint32 LastIndex = Nodes.Num() - 1;
	if(Index != LastIndex)
	{
		for(uint32 ConnectedIndex : Nodes.Last().Connections)
		{
			Nodes[ConnectedIndex].Connections.Remove(LastIndex);
			Nodes[ConnectedIndex].Connections.Add(Index);
		}
	}
	
	Nodes.RemoveAtSwap(Index);
}

void URpSpatialGraphComponent::ConnectNodes(const int32 FirstIndex, const int32 SecondIndex)
{
	if(FirstIndex == SecondIndex)
	{
		return;
	}
	
	Nodes[FirstIndex].Connections.Add(SecondIndex);
	Nodes[SecondIndex].Connections.Add(FirstIndex);
}

void URpSpatialGraphComponent::DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex)
{
	if(FirstIndex == SecondIndex)
	{
		return;
	}
	
	Nodes[FirstIndex].Connections.Remove(SecondIndex);
	Nodes[SecondIndex].Connections.Remove(FirstIndex);
}

void URpSpatialGraphComponent::SetNodeLocation(const int32 Index, const FVector& NewLocation)
{
	Nodes[Index].Location = NewLocation;
}

#if WITH_EDITOR

void URpSpatialGraphComponent::SmoothConnectNodes(const int32 FirstIndex, const int32 SecondIndex, const FVector& ControlPoint)
{
	const FVector& P0 = Nodes[FirstIndex].GetLocation();
	const FVector& P1 = ControlPoint;
	const FVector& P2 = Nodes[SecondIndex].GetLocation();

	TArray<FVector> NewPoints;
	
	const float StepSize = 1 / static_cast<float>(BezierSegments);
	float Alpha = StepSize;
	while(Alpha <= 1.0f - StepSize)
	{
		NewPoints.Add(P1 + FMath::Square(1 - Alpha) * (P0 - P1) + FMath::Square(Alpha) * (P2 - P1));
		Alpha += StepSize;
	}
	
	TArray<int32> NewNodeIndices;
	for(const FVector& NewPoint : NewPoints)
	{
		NewNodeIndices.Add(AddNode(NewPoint));
	}

	ConnectNodes(FirstIndex, NewNodeIndices[0]);
	for(int32 Index = 0; Index < NewNodeIndices.Num() - 1; ++Index)
	{
		ConnectNodes(NewNodeIndices[Index], NewNodeIndices[Index + 1]);
	}
	ConnectNodes(SecondIndex, NewNodeIndices.Last());
}

#endif