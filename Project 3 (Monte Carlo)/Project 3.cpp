// Project 3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "math.h"
#include "iostream"
#include <algorithm>
#include <cmath>
#include "Black_Scholes_option.h"

using namespace std;

//return a uniformly distributed random number
double uniformRandom()
{
	return ((double)(rand()) + 1.) / ((double)(RAND_MAX)+1.);
}

// return a normally distributed random number
double normalRandom()
{
	double u1 = uniformRandom();
	double u2 = uniformRandom();
	return cos(8.*atan(1.)*u2)*sqrt(-2.*log(u1));
}

//Monte Carlo Simulation pricing of an european call option
double EuropeanCallOptionPrice_MonteCarlo(double S0, double K, double expiration, double rf, double vol, double number_of_simulations)
{
	double R = (rf - 0.5 * pow(vol, 2))*expiration;
	double SD = vol*sqrt(expiration);
	double sum_of_payoff = 0.0;
	for (int i = 1; i <= number_of_simulations; i++)
	{
		double ST = S0*exp(R + SD*normalRandom());
		sum_of_payoff = sum_of_payoff + max(0.0, ST - K);
	}
	return exp(-rf*expiration)*(sum_of_payoff / double(number_of_simulations));
}

//Monte Carlo Simulation pricing of an european put option
double EuropeanPutOptionPrice_MonteCarlo(double S0, double K, double expiration, double rf, double vol, double number_of_simulations)
{
	double R = (rf - 0.5 * pow(vol, 2))*expiration;
	double SD = vol*sqrt(expiration);
	double sum_of_payoff = 0.0;
	for (int i = 1; i <= number_of_simulations; i++)
	{
		double ST = S0*exp(R + SD*normalRandom());
		sum_of_payoff = sum_of_payoff + max(0.0, K- ST);
	}
	return exp(-rf*expiration)*(sum_of_payoff / double(number_of_simulations));
}

int main()
{
	//initialize an option
	double S0 = 100;
	double K = 100;
	double expiration = 0.5;
	double rf = 0.05;
	double vol = 0.20;	
	char optionType = 'C';
	Black_Scholes_option option(S0, K, expiration, rf, vol, optionType);

	//print basic information of the option
	cout << "Basic Parameters of the Option: " << endl;
	cout << "Underlying Price: " << S0 << endl;
	cout << "Time to Expiration: " << expiration << endl;
	cout << "Risk-Free Rate: " << rf << endl;
	cout << "Volatility: " << vol << endl;

	//compute price using Monte Carlo simulation
	cout << "\nMonte-Carlo Simulation: \n\n";
	cout << "European Call Price: "<<EuropeanCallOptionPrice_MonteCarlo(S0, K, expiration, rf, vol, 20000)<<endl;
	cout << "European Put Price: "<<EuropeanPutOptionPrice_MonteCarlo(S0, K, expiration, rf, vol, 20000) << endl;

	//compute price and greeks for a call option using Black Scholes Formula
	cout << "\nBlack-Scholes Formula: \n\nOptionType: Call\n";
	cout << "Price: " << option.ComputePrice() << endl;
	cout << "Delta: " << option.ComputeDelta() << endl;
	cout << "Gamma: " << option.ComputeGamma() << endl;
	cout << "Vega:  " << option.ComputeVega() << endl;
	cout << "Rho:   " << option.ComputeRho() << endl;
	cout << "Theta (1d): " << option.ComputeTheta() / 365.0 << endl;

	//change the type of option to put option, then compute price and greeks for this put option
	option.SetOptionType('P');
	cout << "\nOptionType: Put\n";
	cout << "Price: " << option.ComputePrice() << endl;
	cout << "Delta: " << option.ComputeDelta() << endl;
	cout << "Gamma: " << option.ComputeGamma() << endl;
	cout << "Vega:  " << option.ComputeVega() << endl;
	cout << "Rho:   " << option.ComputeRho() << endl;
	cout << "Theta (1d): " << option.ComputeTheta() / 365.0 << endl;

	//checks put call parity of the option
	if (option.CheckPutCallParity())
	{
		cout << "\nPut-Call Parity check succeeded.\n\n";
	}
	else
	{
		cout << "Put-Call Parity check failed.\n\n";
	}
}

