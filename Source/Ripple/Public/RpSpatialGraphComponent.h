// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RpSpatialGraphComponent.generated.h"

// A node has a location and a set of references to every connected node.
UCLASS()
class RIPPLE_API URpSpatialGraphNode : public UObject
{
	GENERATED_BODY()

public:
	
	// World space location of the node
	UPROPERTY(VisibleAnywhere)
	FVector Location = FVector::ZeroVector;

	// Indices of nodes connected to this node
	UPROPERTY(VisibleAnywhere)
	TSet<URpSpatialGraphNode*> Connections;
};

/**
 * The Spatial Graph Component is designed for constructing spatial graphs within a scene. 
 * Each graph created is bi-directional, meaning that connections between nodes allow travel in both directions.
 * Graphs may be cyclic, allowing for the creation of loops within the graph structure.
 * Additionally, the graphs can be disconnected, supporting the creation of multiple, independent graph segments within the same scene.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName = "SpatialGraphComponent", meta = (DisplayName = "SpatialGraphComponent"))
class RIPPLE_API URpSpatialGraphComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URpSpatialGraphComponent();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void CreateGraph();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void DeleteGraph();

	// Returns the number of nodes in the graph
	int32 GetNumNodes() const;

	// Returns the location of node
	FVector GetNodeLocation(const int32 Index) const;

	// Returns a set of indices of nodes connected to a node
	TSet<URpSpatialGraphNode*> GetConnections(const int32 Index) const;

	// Set the location of a node.
	virtual void SetNodeLocation(const int32 Index, const FVector& NewLocation);

	// Checks if an integer is a valid node index
	virtual bool IsValidIndex(const int32 Index) const;

	// Adds a new node to the graph at the specified location and returns its index
	virtual int32 AddNode(const FVector& Location);

	// Severs a nodes connections and then deletes it
	virtual void DeleteNode(const int32 Index);

	// Connects two differnet nodes
	virtual void ConnectNodes(const int32 FirstIndex, const int32 SecondIndex);

	// Disconnects two different nodes
	virtual void DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex);

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

#endif

protected:

	// Array of all nodes in the graph
	UPROPERTY(VisibleAnywhere)
	TArray<URpSpatialGraphNode*> Nodes;
	
};
