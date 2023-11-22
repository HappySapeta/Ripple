// Copyright Anupam Sahu. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

// Vis Proxies are used to detect clicks (or hits) on visualized elements.
struct RIPPLEEDITOR_API HGraphVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	HGraphVisProxy (const UActorComponent* InComponent)
		: HComponentVisProxy(InComponent, HPP_Wireframe)
	{}
};

struct RIPPLEEDITOR_API HNodeVisProxy : public HGraphVisProxy
{
	DECLARE_HIT_PROXY();
	HNodeVisProxy(const UActorComponent* InActorComponent, int32 InSelectedIndex)
		:
		HGraphVisProxy(InActorComponent),
		SelectedIndex(InSelectedIndex)
	{}

	int32 SelectedIndex;
};

/**
 * Component Visualizer for Ripple's Spatial Graph Component.
 *
 * It represents nodes as points and the connections between them as lines.
 * The visualizer offers common graph editing features like adding, removing, joining and disjoining nodes.
 */
class RIPPLEEDITOR_API FRpGraphVisualizer : public FComponentVisualizer
{
public:
	
	// Begin FComponentVisualizer interface
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

	// Handle clicks for node selection
	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;

	// Get location of node that is selected currently
	virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;

	// Apply translation to selected node
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;

	// Consume input for deletion, connection and disconnection commands
	virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key,EInputEvent Event) override;

	// Get currently edited component
	virtual UActorComponent* GetEditedComponent() const override;

private:

	// Currently edited component
	class URpSpatialGraphComponent* GraphComponent = nullptr;
	
	// Index of nodes(s) in currently selected
	int32 FirstSelectedIndex = INDEX_NONE;
	int32 SecondSelectedIndex = INDEX_NONE;
	
	// Flag that prevents unintended node duplication
	bool bAllowDuplication = false;
};
