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

	UPROPERTY(VisibleAnywhere)
	TSet<uint32> Connections;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName = "SpatialGraphComponent", meta = (DisplayName = "SpatialGraphComponent"))
class RIPPLE_API URpSpatialGraphComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	
	// Sets default values for this component's properties
	URpSpatialGraphComponent();

	int32 GetNumNodes() const;

	FVector GetNodeLocation(const int32 Index) const;

	TSet<uint32> GetConnections(const int32 Index) const;

	virtual void SetNodeLocation(const int32 Index, const FVector& NewLocation);

	virtual bool IsValidIndex(const int32 Index) const;

	virtual int32 AddNode(const FVector& Location);

	virtual void DeleteNode(const int32 Index);

	virtual void ConnectNodes(const int32 FirstIndex, const int32 SecondIndex);

	virtual void DisconnectNodes(const int32 FirstIndex, const int32 SecondIndex);

public:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere)
	FLinearColor DebugEdgeColor = FLinearColor::White;

	UPROPERTY(EditAnywhere)
	FLinearColor DebugUnselectedNodeColor = FLinearColor::Black;
	
	UPROPERTY(EditAnywhere)
	FLinearColor DebugSelectedNodeColor = FLinearColor::White;

	UPROPERTY(EditAnywhere)
	float DebugNodeRadius = 10.0f;

	UPROPERTY(EditAnywhere)
	float DebugEdgeThickness = 3.0f;

#endif

protected:

	UPROPERTY(VisibleAnywhere)
	TArray<FRpSpatialGraphNode> Nodes;
};
