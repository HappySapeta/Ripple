#include "GOAP/RpGOAPTypes.h"

int FRpVariantFloat::GetAbsDifference(const FRpVariantBase* Other) const
{
	const FRpVariantFloat* OtherFloat = static_cast<const FRpVariantFloat*>(Other);
	return FMath::Abs(OtherFloat->Value - Value);
}

bool FRpVariantFloat::operator==(const FRpVariantBase* Other) const
{
	const FRpVariantFloat* OtherFloat = static_cast<const FRpVariantFloat*>(Other);
	return Value == OtherFloat->Value;
}

bool FRpVariantFloat::operator<(const FRpVariantBase* Other) const
{
	const FRpVariantFloat* OtherFloat = static_cast<const FRpVariantFloat*>(Other);
	return Value < OtherFloat->Value;
}

bool FRpVariantFloat::operator>(const FRpVariantBase* Other) const
{
	const FRpVariantFloat* OtherFloat = static_cast<const FRpVariantFloat*>(Other);
	return Value > OtherFloat->Value;
}

void FRpVariantFloat::Set(const FRpVariantBase* Other)
{
	Value = static_cast<const FRpVariantFloat*>(Other)->Value;
}

int FRpVariantInteger::GetAbsDifference(const FRpVariantBase* Other) const
{
	const FRpVariantInteger* OtherInt = static_cast<const FRpVariantInteger*>(Other);
	return FMath::Abs(OtherInt->Value - Value);
}

bool FRpVariantInteger::operator==(const FRpVariantBase* Other) const
{
	const FRpVariantInteger* OtherInt = static_cast<const FRpVariantInteger*>(Other);
	return Value == OtherInt->Value;
}

bool FRpVariantInteger::operator<(const FRpVariantBase* Other) const
{
	const FRpVariantInteger* OtherInt = static_cast<const FRpVariantInteger*>(Other);
	return Value < OtherInt->Value;
}

bool FRpVariantInteger::operator>(const FRpVariantBase* Other) const
{
	const FRpVariantInteger* OtherInt = static_cast<const FRpVariantInteger*>(Other);
	return Value > OtherInt->Value;
}

void FRpVariantInteger::Set(const FRpVariantBase* Other)
{
	Value = static_cast<const FRpVariantInteger*>(Other)->Value;
}

int FRpVariantBool::GetAbsDifference(const FRpVariantBase* Other) const
{
	const FRpVariantBool* OtherBool = static_cast<const FRpVariantBool*>(Other);
	return FMath::Abs(static_cast<int>(OtherBool->Value) - static_cast<int>(Value));
}

bool FRpVariantBool::operator==(const FRpVariantBase* Other) const
{
	const FRpVariantBool* OtherBool = static_cast<const FRpVariantBool*>(Other);
	return Value == OtherBool->Value;
}

bool FRpVariantBool::operator<(const FRpVariantBase* Other) const
{
	return false;
}

bool FRpVariantBool::operator>(const FRpVariantBase* Other) const
{
	return false;
}

void FRpVariantBool::Set(const FRpVariantBase* Other)
{
	Value = static_cast<const FRpVariantBool*>(Other)->Value;
}

int FRpVariantVector3::GetAbsDifference(const FRpVariantBase* Other) const
{
	const FRpVariantVector3* OtherVector = static_cast<const FRpVariantVector3*>(Other);
	return FVector3f::Distance(Value, OtherVector->Value);
}

bool FRpVariantVector3::operator==(const FRpVariantBase* Other) const
{
	const FRpVariantVector3* OtherVector = static_cast<const FRpVariantVector3*>(Other);
	return FVector3f::PointsAreNear(Value, OtherVector->Value, 10.0f);
}

bool FRpVariantVector3::operator<(const FRpVariantBase* RpVariantBase) const
{
	return false;
}

bool FRpVariantVector3::operator>(const FRpVariantBase* RpVariantBase) const
{
	return false;
}

void FRpVariantVector3::Set(const FRpVariantBase* Other)
{
	Value = static_cast<const FRpVariantVector3*>(Other)->Value;
}
