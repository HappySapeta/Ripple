// Copyright Anupam Sahu. All Rights Reserved.

#include "RpGraphVisualizer.h"
#include "RpSpatialGraphComponent.h"

IMPLEMENT_HIT_PROXY(HGraphVisProxy, HComponentVisProxy);
IMPLEMENT_HIT_PROXY(HNodeVisProxy, HGraphVisProxy)

void FRpGraphVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	GraphComponent = const_cast<URpSpatialGraphComponent*>(Cast<URpSpatialGraphComponent>(Component));
	if(!GraphComponent)
	{
		return;
	}

	const TArray<FRpSpatialGraphNode>* Nodes = GraphComponent->GetNodes();
	uint32 Index = 0;
	for(const FRpSpatialGraphNode& Node : *Nodes)
	{
		const FVector& NodeLocation = Node.GetLocation();
			
		FLinearColor Color = (Index == FirstSelectedIndex || Index == SecondSelectedIndex) ? GraphComponent->DebugSelectedNodeColor : GraphComponent->DebugUnselectedNodeColor;
		PDI->SetHitProxy(new HNodeVisProxy(Component, Index));
		PDI->DrawPoint(NodeLocation, Color, GraphComponent->DebugNodeRadius, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);
		
		TSet<uint32> Connections = Node.GetConnections();
		for(const uint32 Connection : Connections)
		{
			PDI->DrawLine(NodeLocation, Nodes->operator[](Connection).GetLocation(), GraphComponent->DebugEdgeColor, SDPG_Foreground, GraphComponent->DebugEdgeThickness);
		}

		++Index;
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
	if(IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE && GraphComponent->GetNodes()->IsValidIndex(FirstSelectedIndex))
	{
		OutLocation = GraphComponent->GetNodes()->operator[](FirstSelectedIndex).GetLocation();
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

	if(ViewportClient->IsCtrlPressed() && bAllowDuplication)
	{
		bAllowDuplication = false;
		const FVector& NewLocation = GraphComponent->GetNodes()->operator[](FirstSelectedIndex).GetLocation() + DeltaTranslate;

		const int32 PreviouslySelectedIndex = FirstSelectedIndex;
		FirstSelectedIndex = GraphComponent->AddNode(NewLocation);
		GraphComponent->ConnectNodes(FirstSelectedIndex, PreviouslySelectedIndex);
	}
	else if(ViewportClient->IsShiftPressed() && bAllowDuplication)
	{
		bAllowDuplication = false;
		const FVector& NewLocation = GraphComponent->GetNodes()->operator[](FirstSelectedIndex).GetLocation() + DeltaTranslate;
		FirstSelectedIndex = GraphComponent->AddNode(NewLocation);
	}
	else if(IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE)
	{
		const FVector& NewLocation = GraphComponent->GetNodes()->operator[](FirstSelectedIndex).GetLocation() + DeltaTranslate;
		GraphComponent->SetNodeLocation(FirstSelectedIndex, NewLocation);
	}
	
	return true;
}

bool FRpGraphVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	bool bHandled = false;

	if (ViewportClient->IsCtrlPressed())
	{
		if (IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE && SecondSelectedIndex != INDEX_NONE)
		{
			if(Key == EKeys::X)
			{
				const_cast<URpSpatialGraphComponent*>(GraphComponent)->DisconnectNodes(FirstSelectedIndex, SecondSelectedIndex);
				
				SecondSelectedIndex = INDEX_NONE;
				bHandled = true;
			}
			else if(Key == EKeys::F)
			{
				const_cast<URpSpatialGraphComponent*>(GraphComponent)->ConnectNodes(FirstSelectedIndex, SecondSelectedIndex);
				
				SecondSelectedIndex = INDEX_NONE;
				bHandled = true;
			}
			else if(Key == EKeys::B)
			{
				FVector ControlPoint = ViewportClient->GetCursorWorldLocationFromMousePos().GetOrigin();
				ControlPoint.Z = GraphComponent->GetNodes()->operator[](FirstSelectedIndex).GetLocation().Z;
				const_cast<URpSpatialGraphComponent*>(GraphComponent)->SmoothConnectNodes(FirstSelectedIndex, SecondSelectedIndex, ControlPoint);
            
				FirstSelectedIndex = INDEX_NONE;
				bHandled = true;
			}
		}
	}
	else if (Key == EKeys::X && Event == IE_Released)
	{
		const uint32 NumNodes = GraphComponent->GetNodes()->Num();
		if (IsValid(GraphComponent) && FirstSelectedIndex != INDEX_NONE && NumNodes > 1)
		{
			const_cast<URpSpatialGraphComponent*>(GraphComponent)->DeleteNode(FirstSelectedIndex);
			FirstSelectedIndex = NumNodes != 0 ? NumNodes - 2 : INDEX_NONE;
			bHandled = true;
		}
	}
	else if (Key == EKeys::LeftMouseButton && Event == IE_Released)
	{
		bAllowDuplication = true;
	}

	return bHandled;
}

UActorComponent* FRpGraphVisualizer::GetEditedComponent() const
{
	return GraphComponent;
}
