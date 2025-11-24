// Copyright Anupam Sahu. All Rights Reserved.

#include "GOAP/RpGOAPState.h"

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
