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
	for(uint32 ConnectedIndex : Nodes[Index].Connections)
	{
		Nodes[ConnectedIndex].Connections.Remove(Index);
	}

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

int32 URpSpatialGraphComponent::GetNumNodes() const
{
	return Nodes.Num();
}

bool URpSpatialGraphComponent::IsValidIndex(const int32 Index) const
{
	return Nodes.IsValidIndex(Index);
}

FVector URpSpatialGraphComponent::GetNodeLocation(const int32 Index) const
{
 	return Nodes[Index].Location;
}

TSet<uint32> URpSpatialGraphComponent::GetConnections(const int32 Index) const
{
	return Nodes[Index].Connections;
}

void URpSpatialGraphComponent::SetNodeLocation(const int32 Index, const FVector& NewLocation)
{
	Nodes[Index].Location = NewLocation;
}
