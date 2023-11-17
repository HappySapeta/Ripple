// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

struct EDITOREXTENSIONS_API HGraphVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HGraphVisProxy (const UActorComponent* InComponent)
		: HComponentVisProxy(InComponent, HPP_Wireframe)
	{}
};

struct EDITOREXTENSIONS_API HNodeVisProxy : public HGraphVisProxy
{
	DECLARE_HIT_PROXY();
	HNodeVisProxy(const UActorComponent* InActorComponent, int32 InSelectedIndex)
		:
		HGraphVisProxy(InActorComponent),
		SelectedIndex(InSelectedIndex)
	{}

	int32 SelectedIndex;
};

class EDITOREXTENSIONS_API FRpGraphVisualizer : public FComponentVisualizer
{
public:

	FRpGraphVisualizer();
	
	// Begin FComponentVisualizer interface
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	
	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	
	virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;

private:

	const class URpSpatialGraphComponent* SpatialGraphComponent = nullptr;
	
	// Index of target in selected component
	int32 SelectedNodeIndex;
	
};