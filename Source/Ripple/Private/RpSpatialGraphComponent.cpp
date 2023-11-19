// Copyright Anupam Sahu. All Rights Reserved.

#include "RpSpatialGraphComponent.h"

URpSpatialGraphComponent::URpSpatialGraphComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	URpSpatialGraphComponent::AddNode(FVector(0, 0, 100));
}

int32 URpSpatialGraphComponent::AddNode(const FVector& Location)
{
	const int32 NewNodeIndex = Nodes.Add(FRpSpatialGraphNode());
	SetNodeLocation(NewNodeIndex, Location);
	return NewNodeIndex;
}

void URpSpatialGraphComponent::DeleteNode(const int32 Index)
{
	for(uint32 Connection : Nodes[Index].Connections)
	{
		Nodes[Connection].Connections.Remove(Index);
	}

	Nodes.RemoveAtSwap(Index);
}

void URpSpatialGraphComponent::ConnectNodes(const int32 FirstIndex, const int32 SecondIndex)
{
	Nodes[FirstIndex].Connections.Add(SecondIndex);
	Nodes[SecondIndex].Connections.Add(FirstIndex);
}

void URpSpatialGraphComponent::DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex)
{
	Nodes[FirstIndex].Connections.Remove(SecondIndex);
	Nodes[SecondIndex].Connections.Remove(FirstIndex);
}

int32 URpSpatialGraphComponent::GetNumNodes() const
{
	return Nodes.Num();
}

TSet<uint32> URpSpatialGraphComponent::GetConnections(const int32 Index) const
{
	return Nodes[Index].Connections;
}

bool URpSpatialGraphComponent::IsValidIndex(const int32 Index) const
{
	return Nodes.IsValidIndex(Index);
}

FVector URpSpatialGraphComponent::GetNodeLocation(const int32 Index) const
{
 	return Nodes[Index].Location;
}

void URpSpatialGraphComponent::SetNodeLocation(const int32 Index, const FVector& NewLocation)
{
	Nodes[Index].Location = NewLocation;
}
