// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "RpSpatialGraphComponent.generated.h"

USTRUCT()
struct RIPPLE_API FRpSpatialGraphNode
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FVector Location = FVector::ZeroVector;

	TSet<FRpSpatialGraphNode*> Connections;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName = "SpatialGraphComponent", meta = (DisplayName = "SpatialGraphComponent"))
class RIPPLE_API URpSpatialGraphComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URpSpatialGraphComponent();

	int32 AddNode(const FVector& Location);

	void DeleteNode(const int32 Index);

	void ConnectNodes(const int32 FirstIndex, const int32 SecondIndex);

	void DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex);

	FVector GetNodeLocation(const int32 Index) const;

	void SetNodeLocation(const int32 Index, const FVector& NewLocation);
	
	int32 GetNumNodes() const;

private:

	UPROPERTY(VisibleAnywhere)
	TArray<FRpSpatialGraphNode> Nodes;
};
