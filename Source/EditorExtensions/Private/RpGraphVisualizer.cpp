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
	FirstSelectedIndex = INDEX_NONE;
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
			
		FLinearColor Color = (Index == FirstSelectedIndex || Index == SecondSelectedIndex) ? FLinearColor::White : FLinearColor::Gray;
		PDI->SetHitProxy(new HNodeVisProxy(Component, Index));
		PDI->DrawPoint(NodeLocation, Color, 20.0f, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);

		TSet<uint32> Connections = SpatialGraphComponent->GetConnections(Index);
		for(const uint32 Connection : Connections)
		{
			// Note deletion and DrawLine might be called asynchronously causing an exception where supplied index is invalid.
			if(!SpatialGraphComponent->IsValidIndex(Connection))
			{
				continue;
			}
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
			if(InViewportClient->IsCtrlPressed() && FirstSelectedIndex != INDEX_NONE)
			{
				SecondSelectedIndex = Proxy->SelectedIndex;
			}
			else
			{
				FirstSelectedIndex = Proxy->SelectedIndex;
			}
		}
	}
	else
	{
		SecondSelectedIndex = INDEX_NONE;
		FirstSelectedIndex = INDEX_NONE;
	}

	return bEditing;
}

bool FRpGraphVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if(IsValid(SpatialGraphComponent) && FirstSelectedIndex != INDEX_NONE)
	{
		OutLocation = SpatialGraphComponent->GetNodeLocation(FirstSelectedIndex);
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
		const FVector& NewLocation = SpatialGraphComponent->GetNodeLocation(FirstSelectedIndex) + DeltaTranslate;

		const int32 PreviouslySelectedIndex = FirstSelectedIndex;
		FirstSelectedIndex = const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->AddNode(NewLocation);
		const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->ConnectNodes(FirstSelectedIndex, PreviouslySelectedIndex);
	}
	
	if(IsValid(SpatialGraphComponent) && FirstSelectedIndex != INDEX_NONE)
	{
		const FVector& NewLocation = SpatialGraphComponent->GetNodeLocation(FirstSelectedIndex) + DeltaTranslate;
		const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->SetNodeLocation(FirstSelectedIndex, NewLocation);
		bHandled = true;
	}
	
	return bHandled;
}

bool FRpGraphVisualizer::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	bool bHandled = false;

	if(ViewportClient->IsAltPressed() && Key == EKeys::X && Event == IE_Released)
	{
		if(IsValid(SpatialGraphComponent) && FirstSelectedIndex != INDEX_NONE && SecondSelectedIndex != INDEX_NONE)
		{
			const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->DisconnectNodes(FirstSelectedIndex, SecondSelectedIndex);
			SecondSelectedIndex = INDEX_NONE;
			bHandled = true;
		}
	}
	else if(Key == EKeys::X && Event == IE_Released)
	{
		if(IsValid(SpatialGraphComponent) && FirstSelectedIndex != INDEX_NONE && SpatialGraphComponent->GetNumNodes() > 1)
		{
			const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->DeleteNode(FirstSelectedIndex);
			FirstSelectedIndex = SpatialGraphComponent->GetNumNodes() != 0 ? SpatialGraphComponent->GetNumNodes() - 1 : INDEX_NONE;
			bHandled = true;
		}
	}

	if(ViewportClient->IsCtrlPressed() && Key == EKeys::F && Event == IE_Released)
	{
		if(IsValid(SpatialGraphComponent) && FirstSelectedIndex != INDEX_NONE && SecondSelectedIndex != INDEX_NONE)
		{
			const_cast<URpSpatialGraphComponent*>(SpatialGraphComponent)->ConnectNodes(FirstSelectedIndex, SecondSelectedIndex);
			SecondSelectedIndex = INDEX_NONE;
			bHandled = true;
		}
	}

	if(Key == EKeys::LeftMouseButton && Event == IE_Released)
	{
		bAllowDuplication = true;
	}

	return bHandled;
}
