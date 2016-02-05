#include "stdafx.h"
#include "Black_Scholes_option.h"
#include "cmath"

using namespace std;

Black_Scholes_option::Black_Scholes_option(double S0, double K, double expiration, double rf, double vol, char optionType)
{
	this->S0 = S0;
	this->K = K;
	this->expiration = expiration;
	this->rf = rf;
	this->vol = vol;
	this->optionType = optionType;
}

//compute normal distribution CDF using Hastings approximation
double Black_Scholes_option::NormalCDF(double x)
{
	double a1 = 0.254829592;
	double a2 = -0.284496736;
	double a3 = 1.421413741;
	double a4 = -1.453152027;
	double a5 = 1.061405429;
	double p = 0.3275911;

	int sign = 1;
	if (x < 0)
		sign = -1;
	x = fabs(x) / sqrt(2.0);

	// A&S formula 7.1.26
	double t = 1.0 / (1.0 + p*x);
	double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

	return 0.5*(1.0 + sign*y);
}

double Black_Scholes_option::NormalPDF(double x) {
	return exp(-x*x / 2.0) / sqrt(2.0*3.14159265359);
}

//compute price using Black Scholes Formula
double Black_Scholes_option::ComputePrice()
{
	double d = (log(S0 / K) + expiration*(rf + 0.5*vol*vol)) / (vol*sqrt(expiration));
	if (optionType == 'C') {
		return S0*NormalCDF(d) - exp(-rf*expiration)*K*NormalCDF(d - vol*sqrt(expiration));;
	}
	else {
		return S0*NormalCDF(d) - exp(-rf*expiration)*K*NormalCDF(d - vol*sqrt(expiration))
			- S0 + K*exp(-rf*expiration);
	}
}

//compute Delta using Black Scholes Formula
double Black_Scholes_option::ComputeDelta()
{
	double d = (log(S0 / K) + expiration*(rf + 0.5*vol*vol)) / (vol*sqrt(expiration));
	if (optionType == 'C') {
		return NormalCDF(d);
	}
	else {
		return NormalCDF(d) - 1;
	}
}

//compute Gamma using Black Scholes Formula
double Black_Scholes_option::ComputeGamma()
{
	double d = (log(S0 / K) + expiration*(rf + 0.5*vol*vol)) / (vol*sqrt(expiration));
	return NormalPDF(d) / S0 / vol / sqrt(expiration);
}

//compute Vega using Black Scholes Formula
double Black_Scholes_option::ComputeVega()
{
	double d = (log(S0 / K) + expiration*(rf + 0.5*vol*vol)) / (vol*sqrt(expiration));
	return S0*NormalPDF(d)*sqrt(expiration);
}

//compute Rho using Black Scholes Formula
double Black_Scholes_option::ComputeRho()
{
	double d = (log(S0 / K) + expiration*(rf + 0.5*vol*vol)) / (vol*sqrt(expiration));
	if (optionType == 'C') {
		return expiration*K*exp(-rf*expiration)*NormalCDF(d - vol*sqrt(expiration));
	}
	else {
		return -expiration*K*exp(-rf*expiration)*NormalCDF(vol*sqrt(expiration) - d);
	}
}
//compute Theta using Black Scholes Formula
double Black_Scholes_option::ComputeTheta()
{
	double d = (log(S0 / K) + expiration*(rf + 0.5*vol*vol)) / (vol*sqrt(expiration));
	if (optionType == 'C') {
		return -S0*NormalPDF(d)*vol / 2 / sqrt(expiration) - rf*K*exp(-rf*expiration)*NormalCDF(d - vol*sqrt(expiration));
	}
	else {
		return -S0*NormalPDF(d)*vol / 2 / sqrt(expiration) + rf*K*exp(-rf*expiration)*NormalCDF(vol*sqrt(expiration) - d);
	}
}

//check put-call parity of this option
bool Black_Scholes_option::CheckPutCallParity()
{
	SetOptionType('C');
	double C = ComputePrice();
	SetOptionType('P');
	double P = ComputePrice();

	double epsilon = 0.0001;
	
	return C - P - S0 + K*exp(-rf*expiration) < epsilon;
}