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

int32 URpSpatialGraphComponent::AddNode(const FVector& Location)
{
	const int32 NewNodeIndex = Nodes.Add(NewObject<URpSpatialGraphNode>(this));
	SetNodeLocation(NewNodeIndex, Location);
	return NewNodeIndex;
}

void URpSpatialGraphComponent::DeleteNode(const int32 Index)
{
	// Sever all connections
	for(URpSpatialGraphNode* Node : Nodes[Index]->Connections)
	{
		Node->Connections.Remove(Nodes[Index]);
	}

	// Delete node
   	Nodes.RemoveAtSwap(Index);
}

void URpSpatialGraphComponent::ConnectNodes(const int32 FirstIndex, const int32 SecondIndex)
{
	if(FirstIndex == SecondIndex)
	{
		return;
	}
	
	Nodes[FirstIndex]->Connections.Add(Nodes[SecondIndex]);
	Nodes[SecondIndex]->Connections.Add(Nodes[FirstIndex]);
}

void URpSpatialGraphComponent::DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex)
{
	if(FirstIndex == SecondIndex)
	{
		return;
	}
	
	Nodes[FirstIndex]->Connections.Remove(Nodes[SecondIndex]);
	Nodes[SecondIndex]->Connections.Remove(Nodes[FirstIndex]);
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
 	return Nodes[Index]->Location;
}

TSet<const URpSpatialGraphNode*> URpSpatialGraphComponent::GetConnections(const int32 Index) const
{
	TSet<const URpSpatialGraphNode*> ConstConverted;
	ConstConverted.Reserve(Nodes[Index]->Connections.Num());

	for(URpSpatialGraphNode* Connection : Nodes[Index]->Connections)
	{
		ConstConverted.Add(Connection);
	}

	return ConstConverted;
}

void URpSpatialGraphComponent::SetNodeLocation(const int32 Index, const FVector& NewLocation)
{
	Nodes[Index]->Location = NewLocation;
}
