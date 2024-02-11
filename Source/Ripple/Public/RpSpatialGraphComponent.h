// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RpSpatialGraphComponent.generated.h"

// A node has a location and a set of references in the form of indices, to every connected node.
USTRUCT()
struct RIPPLE_API FRpSpatialGraphNode
{
	GENERATED_BODY()

	friend class URpSpatialGraphComponent;

public:

	FVector GetLocation() const
	{
		return Location;
	}

	TSet<uint32> GetConnections() const
	{
		return Connections;
	}
	
private:
	
	// World space location of the node
	UPROPERTY(EditAnywhere)
	FVector Location = FVector::ZeroVector;

	// Indices of nodes connected to this node
	UPROPERTY(VisibleAnywhere)
	TSet<uint32> Connections;
};

/**
 * The Spatial Graph Component is designed for constructing spatial graphs within a scene. 
 * Each graph created is bi-directional, meaning that connections between nodes allow travel in both directions always.
 * Graphs may be cyclic, allowing for the creation of loops within the graph structure.
 * Additionally, the graphs can be disconnected, supporting the creation of multiple, independent graph segments within the same scene.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName = "SpatialGraphComponent", meta = (DisplayName = "SpatialGraphComponent"))
class RIPPLE_API URpSpatialGraphComponent : public USceneComponent
{
	GENERATED_BODY()

	// Currently graph modification is only allowed from the Graph Visualizer.
	friend class FRpGraphVisualizer;
	
public:
	
	// Sets default values for this component's properties
	URpSpatialGraphComponent();

	// Return all nodes in the graph
	const TArray<FRpSpatialGraphNode>* GetNodes() const;

protected:

	// Adds one node to the graph 
	UFUNCTION(BlueprintCallable, CallInEditor)
	void CreateGraph();

	// Deletes all nodes effectively deleting the entire graph
	UFUNCTION(BlueprintCallable, CallInEditor)
	void DeleteGraph();
	
	// Set the location of a node.
	void SetNodeLocation(const int32 Index, const FVector& NewLocation);
	
	// Adds a new node to the graph at the specified location and returns its index
	int32 AddNode(const FVector& Location);

	// Severs a nodes connections and then deletes it
	void DeleteNode(const int32 Index);

	// Connects two different nodes
	void ConnectNodes(const int32 FirstIndex, const int32 SecondIndex);

	// Disconnects two different nodes
	void DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex);

	void SmoothConnectNodes(const int32 FirstIndex, const int32 SecondIndex, const FVector& ControlPoint);

	void BezierCurve(const FVector& P1, const FVector& P2, const FVector& P0);

public:

#if WITH_EDITORONLY_DATA

	// Color of lines representing edges in the graph
	UPROPERTY(EditAnywhere, Category = Debug)
	FLinearColor DebugEdgeColor = FLinearColor::White;

	// Color of unselected graph node / key
	UPROPERTY(EditAnywhere, Category = Debug)
	FLinearColor DebugUnselectedNodeColor = FLinearColor::Black;

	// Color of selected graph node / key
	UPROPERTY(EditAnywhere, Category = Debug)
	FLinearColor DebugSelectedNodeColor = FLinearColor::White;

	// Size of points that represent nodes
	UPROPERTY(EditAnywhere, Category = Debug)
	float DebugNodeRadius = 10.0f;

	// Thickness of lines representing edges in the graph
	UPROPERTY(EditAnywhere, Category = Debug)
	float DebugEdgeThickness = 3.0f;

	UPROPERTY(EditAnywhere, Category = Connections)
	int32 BezierSegments = 2;

#endif

protected:

	// Array of all nodes in the graph
	UPROPERTY(EditFixedSize, EditAnywhere)
	TArray<FRpSpatialGraphNode> Nodes;
	
};
