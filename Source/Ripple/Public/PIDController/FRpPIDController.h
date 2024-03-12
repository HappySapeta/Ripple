#pragma once

template <typename T>
class FRpPIDController
{
public:

	FRpPIDController(const T& InitialError)
		: PreviousError(InitialError)
	{}
	
	void Tune(const float ProportionalConstant, const float IntegralConstant, const float DerivativeConstant)
	{
		Kp = ProportionalConstant;
		Ki = IntegralConstant;
		Kd = DerivativeConstant;
	}
	
	T Evaluate(const T& Error, const float DeltaTime)
	{
		T result = GetProportionalComponent(Error) + GetIntegralComponent(Error, DeltaTime) + GetDerivativeComponent(Error, DeltaTime);
		PreviousError = Error;
		return result;
	}

	T GetProportionalComponent(const T& Error)
	{
		return Kp * Error;
	}

	T GetIntegralComponent(const T& Error, const float DeltaTime)
	{
		return Ki * (Error + PreviousError) * DeltaTime;
	}

	T GetDerivativeComponent(const T& Error, const float DeltaTime)
	{
		return Kd * ((Error - PreviousError) / DeltaTime);
	}
	
private:

	T PreviousError = T {};
	
	float Kp = 0.0f;
	float Ki = 0.0f;
	float Kd = 0.0f;
	
};
