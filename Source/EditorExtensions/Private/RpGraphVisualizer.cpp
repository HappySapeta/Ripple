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
	const URpSpatialGraphComponent* GraphComponent = Cast<URpSpatialGraphComponent>(Component);
	if(!GraphComponent)
	{
		return;
	}

	const int32 NumNodes = GraphComponent->GetNumNodes();
	for(int Index = 0; Index < NumNodes; ++Index)
	{
		const FVector& NodeLocation = GraphComponent->GetNodeLocation(Index);
			
		FLinearColor Color = (Index == SelectedNodeIndex) ? FLinearColor::White : FLinearColor::Gray;
		PDI->SetHitProxy(new HNodeVisProxy(Component, Index));
		PDI->DrawPoint(NodeLocation, Color, 20.0f, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);

		if(Index < NumNodes - 1)
		{
			const FVector& NextNodeLocation = GraphComponent->GetNodeLocation(Index + 1);
			PDI->DrawLine(NodeLocation, NextNodeLocation, FLinearColor::Blue, SDPG_Foreground, 1.0f);
		}
	}
}

bool FRpGraphVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	bool bEditing = false;

	if(VisProxy && VisProxy->Component.IsValid())
	{
		bEditing = true;
		SpatialGraphComponent = static_cast<const URpSpatialGraphComponent*>(VisProxy->Component.Get());
		if(VisProxy->IsA(HNodeVisProxy::StaticGetType()))
		{
			HNodeVisProxy* Proxy = static_cast<HNodeVisProxy*>(VisProxy);
			SelectedNodeIndex = Proxy->SelectedIndex;
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

	if(IsValid(SpatialGraphComponent) && SelectedNodeIndex != INDEX_NONE)
	{
		const FVector& NewLocation = SpatialGraphComponent->GetNodeLocation(SelectedNodeIndex) + DeltaTranslate;
		SpatialGraphComponent->SetNodeLocation(SelectedNodeIndex, NewLocation);
		bHandled = true;
	}
	
	return bHandled;
}

