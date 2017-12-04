#ifndef FUNCTION_H
#define FUNCTION_H

#include <math.h>
#include <iostream>
#include <gsl/gsl_sf_gamma.h>
using namespace::std;

double Stopping_power_Si(double E);
double Stopping_power_CH2(double E);
double Energy_Loss_Si(double E, double th);
double Energy_Loss_CH2(double E, double th);
double Fermi_function(double e);
double Beta_spectrum(double K);
double Beta_Stopping_Power(double T);
double Energy_Loss_Beta(double E, double th);
//NewFunction

#endif //FUNCTION_H
