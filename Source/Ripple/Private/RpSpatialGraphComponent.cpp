// Copyright Anupam Sahu. All Rights Reserved.

#include "RpSpatialGraphComponent.h"

URpSpatialGraphComponent::URpSpatialGraphComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	AddNode(FVector::Zero());
}

int32 URpSpatialGraphComponent::AddNode(const FVector& Location)
{
	const int32 NewNodeIndex = Nodes.Add(MakeUnique<FRpSpatialGraphNode>());
	Nodes.Last()->Location = Location;
	return NewNodeIndex;
}

void URpSpatialGraphComponent::DeleteNode(const int32 Index)
{
	const FRpSpatialGraphNode* NodeToBeRemoved = Nodes[Index].Get();
	for(FRpSpatialGraphNode* Connection : NodeToBeRemoved->Connections)
	{
		Connection->Connections.Remove(NodeToBeRemoved);
	}

	Nodes.RemoveAt(Index);
}

void URpSpatialGraphComponent::ConnectNodes(const int32 FirstIndex, const int32 SecondIndex) const
{
	FRpSpatialGraphNode* FirstNode = Nodes[FirstIndex].Get();
	FRpSpatialGraphNode* SecondNode = Nodes[SecondIndex].Get();

	FirstNode->Connections.Add(SecondNode);
	SecondNode->Connections.Add(FirstNode);
}

void URpSpatialGraphComponent::DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex) const
{
	FRpSpatialGraphNode* FirstNode = Nodes[FirstIndex].Get();
	FRpSpatialGraphNode* SecondNode = Nodes[SecondIndex].Get();

	FirstNode->Connections.Remove(SecondNode);
	SecondNode->Connections.Remove(FirstNode);
}

int32 URpSpatialGraphComponent::GetNumNodes() const
{
	return Nodes.Num();
}

FVector URpSpatialGraphComponent::GetNodeLocation(const int32 Index) const
{
	return Nodes[Index]->Location;
}

void URpSpatialGraphComponent::SetNodeLocation(const int32 Index, const FVector& NewLocation) const
{
	Nodes[Index]->Location = NewLocation;
}
