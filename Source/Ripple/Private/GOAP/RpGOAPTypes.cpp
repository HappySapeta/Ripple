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
