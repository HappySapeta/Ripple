// Copyright Anupam Sahu. All Rights Reserved.

#include "RpGraphVisualizer.h"
#include "RpSpatialGraphComponent.h"

#if WITH_EDITOR
#define LOCTEXT_NAMESPACE "RippleEditorExtensions"
#endif

IMPLEMENT_HIT_PROXY(HGraphVisProxy, HComponentVisProxy);
IMPLEMENT_HIT_PROXY(HNodeVisProxy, HGraphVisProxy)

void FRpGraphVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	GraphComponent = const_cast<URpSpatialGraphComponent*>(Cast<URpSpatialGraphComponent>(Component));
	if(!GraphComponent)
	{
		return;
	}
	
	const int32 NumNodes = GraphComponent->GetNumNodes();
	for(int Index = 0; Index < NumNodes; ++Index)
	{
		const FVector& NodeLocation = GraphComponent->GetNodeLocation(Index);
			
		FLinearColor Color = (Index == FirstSelectedIndex || Index == SecondSelectedIndex) ? FLinearColor::White : FLinearColor::Black;
		PDI->SetHitProxy(new HNodeVisProxy(Component, Index));
		PDI->DrawPoint(NodeLocation, Color, 20.0f, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);

		TSet<uint32> Connections = GraphComponent->GetConnections(Index);
		for(const uint32 Connection : Connections)
		{
			// Note deletion and DrawLine might be called asynchronously causing an exception where supplied index is invalid.
			if(!GraphComponent->IsValidIndex(Connection))
			{
				continue;
			}
			PDI->DrawLine(NodeLocation, GraphComponent->GetNodeLocation(Connection), FLinearColor::White, SDPG_Foreground, 2.0f);
		}
	}
}

bool FRpGraphVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	if(!VisProxy || !VisProxy->Component.IsValid() || !VisProxy->IsA(HNodeVisProxy::StaticGetType()))
	{
		SecondSelectedIndex = INDEX_NONE;
		FirstSelectedIndex = INDEX_NONE;
		return false;
	}
		
	HNodeVisProxy* Proxy = static_cast<HNodeVisProxy*>(VisProxy);
	if(InViewportClient->IsCtrlPressed() && FirstSelectedIndex != INDEX_NONE)
	{
		SecondSelectedIndex = Proxy->SelectedIndex;
	}
	else
	{
		FirstSelectedIndex = Proxy->SelectedIndex;
	}

	return true;
}

bool FRpGraphVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if(IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE)
	{
		OutLocation = GraphComponent->GetNodeLocation(FirstSelectedIndex);
		return true;
	}

	return false;
}

bool FRpGraphVisualizer::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale)
{
	if(!IsValid(GraphComponent))
	{
		return false;
	}

	if(ViewportClient->IsAltPressed() && bAllowDuplication)
	{
		bAllowDuplication = false;
		const FVector& NewLocation = GraphComponent->GetNodeLocation(FirstSelectedIndex) + DeltaTranslate;

		const int32 PreviouslySelectedIndex = FirstSelectedIndex;
		FirstSelectedIndex = GraphComponent->AddNode(NewLocation);
		GraphComponent->ConnectNodes(FirstSelectedIndex, PreviouslySelectedIndex);
	}
	else if(IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE)
	{
		const FVector& NewLocation = GraphComponent->GetNodeLocation(FirstSelectedIndex) + DeltaTranslate;
		GraphComponent->SetNodeLocation(FirstSelectedIndex, NewLocation);
	}
	
	return true;
}

bool FRpGraphVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	bool bHandled = false;

	if(ViewportClient->IsAltPressed() && Key == EKeys::X && Event == IE_Released)
	{
		if(IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE && SecondSelectedIndex != INDEX_NONE)
		{
			const_cast<URpSpatialGraphComponent*>(GraphComponent)->DisconnectNodes(FirstSelectedIndex, SecondSelectedIndex);
			SecondSelectedIndex = INDEX_NONE;
			bHandled = true;
		}
	}
	else if(Key == EKeys::X && Event == IE_Released)
	{
		if(IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE && GraphComponent->GetNumNodes() > 1)
		{
			const_cast<URpSpatialGraphComponent*>(GraphComponent)->DeleteNode(FirstSelectedIndex);
			FirstSelectedIndex = GraphComponent->GetNumNodes() != 0 ? GraphComponent->GetNumNodes() - 1 : INDEX_NONE;
			bHandled = true;
		}
	}
	else if(ViewportClient->IsCtrlPressed() && Key == EKeys::F && Event == IE_Released)
	{
		if(IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE && SecondSelectedIndex != INDEX_NONE)
		{
			const_cast<URpSpatialGraphComponent*>(GraphComponent)->ConnectNodes(FirstSelectedIndex, SecondSelectedIndex);
			SecondSelectedIndex = INDEX_NONE;
			bHandled = true;
		}
	}
	else if(Key == EKeys::LeftMouseButton && Event == IE_Released)
	{
		bAllowDuplication = true;
	}

	return bHandled;
}

UActorComponent* FRpGraphVisualizer::GetEditedComponent() const
{
	return GraphComponent;
}
