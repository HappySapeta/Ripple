// Copyright Anupam Sahu. All Rights Reserved.

#include "StateMachine/RpState.h"

void URpState::Activate()
{
	OnActivate();
}

void URpState::DeActivate()
{
	OnDeactivate();
}
