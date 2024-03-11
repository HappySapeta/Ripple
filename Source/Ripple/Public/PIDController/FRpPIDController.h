#pragma once

template <typename T>
class FRpPIDController
{
public:

	FRpPIDController() = default;
	
	FRpPIDController(const T* TargetVar, const float ProportionalConstant, const float IntegralConstant, const float DerivativeConstant)
		:Target(TargetVar), Kp(ProportionalConstant), Ki(IntegralConstant), Kd(DerivativeConstant)
	{}
	
	T Evaluate(const T& Input, const float DeltaTime)
	{
		return GetProportionalComponent(Input) + GetIntegralComponent(Input, DeltaTime) + GetDerivativeComponent(Input, DeltaTime);
	}

	T GetProportionalComponent(const T& Input)
	{
		return Kp * GetError(Input);
	}

	T GetIntegralComponent(const T& Input, const float DeltaTime)
	{
		return Ki * GetError(Input) * DeltaTime;
	}

	T GetDerivativeComponent(const T& Input, const float DeltaTime)
	{
		return Kd * (GetError(Input) / DeltaTime);
	}
	
private:

	T GetError(const T& Input)
	{
		return Input - *Target;
	}
	
private:

	const T* Target = nullptr;

	float Kp = 0.0f;
	float Ki = 0.0f;
	float Kd = 0.0f;
	
};
