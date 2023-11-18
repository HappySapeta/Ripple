// Copyright Anupam Sahu. All Rights Reserved.

#include "RpGraphVisualizer.h"
#include "RpSpatialGraphComponent.h"

#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "RippleEditorExtensions"
#endif

IMPLEMENT_HIT_PROXY(HGraphVisProxy, HComponentVisProxy);
IMPLEMENT_HIT_PROXY(HNodeVisProxy, HGraphVisProxy)

FRpGraphVisualizer::FRpGraphVisualizer()
{
	SelectedNodeIndex = INDEX_NONE;
}

void FRpGraphVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	SpatialGraphComponent = Cast<URpSpatialGraphComponent>(Component);
	if(!SpatialGraphComponent)
	{
		return;
	}
	
	const int32 NumNodes = SpatialGraphComponent->GetNumNodes();
	for(int Index = 0; Index < NumNodes; ++Index)
	{
		const FVector& NodeLocation = SpatialGraphComponent->GetNodeLocation(Index);
			
		FLinearColor Color = (Index == SelectedNodeIndex) ? FLinearColor::White : FLinearColor::Yellow;
		PDI->SetHitProxy(new HNodeVisProxy(Component, Index));
		PDI->DrawPoint(NodeLocation, Color, 20.0f, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);

		TSet<uint32> Connections = SpatialGraphComponent->GetConnections(Index);
		for(const uint32 Connection : Connections)
		{
			PDI->DrawLine(NodeLocation, SpatialGraphComponent->GetNodeLocation(Connection), FLinearColor::Blue, SDPG_Foreground, 1.0f);
		}
	}
}

bool FRpGraphVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	bool bEditing = false;

	if(VisProxy && VisProxy->Component.IsValid())
	{
		bEditing = true;
		if(VisProxy->IsA(HNodeVisProxy::StaticGetType()))
		{
			HNodeVisProxy* Proxy = static_cast<HNodeVisProxy*>(VisProxy);
			SelectedNodeIndex = Proxy->SelectedIndex;
			UE_LOG(LogTemp, Warning, TEXT("Selected node index %d."), SelectedNodeIndex);
		}
	}
	else
	{
		SelectedNodeIndex = INDEX_NONE;
	}

	return bEditing;
}

bool FRpGraphVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if(IsValid(SpatialGraphComponent) && SelectedNodeIndex != INDEX_NONE)
	{
		OutLocation = SpatialGraphComponent->GetNodeLocation(SelectedNodeIndex);
		return true;
	}

	return false;
}

bool FRpGraphVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale)
{
	bool bHandled = false;

	if(ViewportClient->IsAltPressed() && bAllowDuplication)
	{
		bAllowDuplication = false;
		const FVector& NewLocation = SpatialGraphComponent->GetNodeLocation(SelectedNodeIndex) + DeltaTranslate;

		const int32 PreviouslySelectedIndex = SelectedNodeIndex;
		SelectedNodeIndex = const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->AddNode(NewLocation);
		const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->ConnectNodes(SelectedNodeIndex, PreviouslySelectedIndex);
	}
	
	if(IsValid(SpatialGraphComponent) && SelectedNodeIndex != INDEX_NONE)
	{
		const FVector& NewLocation = SpatialGraphComponent->GetNodeLocation(SelectedNodeIndex) + DeltaTranslate;
		const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->SetNodeLocation(SelectedNodeIndex, NewLocation);
		bHandled = true;
	}
	
	return bHandled;
}

bool FRpGraphVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	bool bHandled = false;
	if(Key == EKeys::Delete && Event == IE_Released)
	{
		if(IsValid(SpatialGraphComponent) && SelectedNodeIndex != INDEX_NONE)
		{
			const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->DeleteNode(SelectedNodeIndex);
			SelectedNodeIndex = SpatialGraphComponent->GetNumNodes() != 0 ? SpatialGraphComponent->GetNumNodes() - 1 : INDEX_NONE;
			bHandled = true;
		}
	}

	if(Key == EKeys::LeftMouseButton && Event == IE_Released)
	{
		bAllowDuplication = true;
	}

	return bHandled;
}
