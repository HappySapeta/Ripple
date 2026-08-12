#pragma once

#include "Math/Vector2D.h"

struct FRpMath
{
	[[nodiscard]] static UE_FORCEINLINE_HINT float GetSignedAngleDegrees(FVector2f V, FVector2f W)
	{
		V.Normalize();
		W.Normalize();
		return FMath::RadiansToDegrees(FMath::Atan2((V.X * W.Y) - (V.Y * W.X), (V.X * W.X) + (V.Y * W.Y)));
	};
};

