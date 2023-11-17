// Copyright Anupam Sahu. All Rights Reserved.

#include "RpSpatialGraphComponent.h"

URpSpatialGraphComponent::URpSpatialGraphComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	AddNode(FVector(0, 0, 100));
}

int32 URpSpatialGraphComponent::AddNode(const FVector& Location)
{
	const int32 NewNodeIndex = Nodes.Add(FRpSpatialGraphNode());
	SetNodeLocation(NewNodeIndex, Location);
	return NewNodeIndex;
}

void URpSpatialGraphComponent::DeleteNode(const int32 Index)
{
	const FRpSpatialGraphNode& NodeToBeRemoved = Nodes[Index];
	for(FRpSpatialGraphNode* Connection : NodeToBeRemoved.Connections)
	{
		Connection->Connections.Remove(&NodeToBeRemoved);
	}

	Nodes.RemoveAt(Index);
}

void URpSpatialGraphComponent::ConnectNodes(const int32 FirstIndex, const int32 SecondIndex)
{
	Nodes[FirstIndex].Connections.Add(&Nodes[SecondIndex]);
	Nodes[SecondIndex].Connections.Add(&Nodes[FirstIndex]);
}

void URpSpatialGraphComponent::DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex)
{
	Nodes[FirstIndex].Connections.Remove(&Nodes[SecondIndex]);
	Nodes[SecondIndex].Connections.Remove(&Nodes[FirstIndex]);
}

int32 URpSpatialGraphComponent::GetNumNodes() const
{
	return Nodes.Num();
}

FVector URpSpatialGraphComponent::GetNodeLocation(const int32 Index) const
{
	return Nodes[Index].Location;
}

void URpSpatialGraphComponent::SetNodeLocation(const int32 Index, const FVector& NewLocation)
{
	Nodes[Index].Location = NewLocation;
}
