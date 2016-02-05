#pragma once
class Black_Scholes_option
{
public:
	//constructor to initialize an option
	Black_Scholes_option(double, double, double, double, double, char);
	//simple functions to get/set values of an option
	void SetS0(const double S0) { this->S0 = S0; }
	double GetS0() const { return S0; }
	void SetK(const double K) { this->K = K; }
	double GetK() const { return K; }
	void SetExpiration(const double expiration) { this->expiration = expiration; }
	double GetExpiration() const { return expiration; }
	void SetRf(const double rf) { this->rf = rf; }
	double GetRf() const { return rf; }
	void SetVol(const double vol) { this->vol = vol; }
	double GetVol() const { return vol; }
	void SetOptionType(const char optionType) { this->optionType = optionType; }
	double GetOptionType() const { return optionType; }
	
	//check put-call parity of this option
	bool CheckPutCallParity();

	// compute price
	double ComputePrice();

	// compute greeks
	double ComputeDelta();
	double ComputeGamma();
	double ComputeRho();
	double ComputeVega();
	double ComputeTheta();

	//compute CDF of normal distribution
	double NormalCDF(double);
	double NormalPDF(double);

private:
	//inputs of Black Scholes Formula
	double S0;//current price
	double K;//strike price
	double expiration;//time to expiration
	double rf;//risk free rate
	double vol;//volatility
	char optionType;//"C" or "P"
};

