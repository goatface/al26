#include<Functions.h>

double a = 1/137.036;

double Fermi_function(double e){
  double S;
  double p;
  double eta;
  double value;
  gsl_sf_result lnr,arg;
  S = sqrt(1-169*a*a);
  p = sqrt(e*e-511*511);
  eta=-a*13*e/sqrt(e*e-511*511);
  gsl_sf_lngamma_complex_e(S,eta,&lnr,&arg);
  value=2*(1+S)/pow(gsl_sf_gamma(1+2*S),2)*pow(p,2*S-2)*exp(M_PI*eta)*pow(exp(lnr.val),2);
  //note: ρ and other coefficient = 1 because of normalization
  //OR
  //value=exp(2*M_PI*eta)/(1+exp(2*M_PI+eta))/0.00160092;
  return value;
}

double Beta_spectrum(double K){
  double N,E;
  E = K + 511;
  N=Fermi_function(E)*sqrt(E*E-511*511)*E/0.000556212/1906.18/1.39521e+16*(4004.4+228.30513 -E-511)*(4004.4+228.30513 -E-511);
  if(K>0.0&&K<3210.70531){
  return N;
  } else {
  return 0;
  }
}

double Beta_Stopping_Power(double T){


/* This stopping power is for collisional stopping power with regardless of radiative stopping power (radiative stopping power is important for >10MeV ) 
 * Radiative stopping power is same as electrons but collisional stopping power. */

    double dEdx, beta2, I, e2, mc2, N, A, Z, r , rho, NA, chi, gamma, fpGamma, E;


/*    for Silicon    */

    I = 173e-3; //keV  = 173eV
    Z = 14;
    A = 28; // g/mol
    rho = 2.329; // g/cm3


    NA = 6.023e23; // 1/mol
    e2 = 1.44e-10; // keV*cm
    mc2 = 511.0; // keV
    E = T + mc2; // keV
    beta2 = 1 - pow(mc2/(T+mc2),2); // 1 > beta2 > 0
    r = e2/mc2; // cm
    chi = 4*M_PI*r*r*mc2/beta2; // keV^2*cm^2
    gamma = E/mc2; // gamma > 1

/*

   E = mc2 + T = m*gamma*c^2 = mc2(1+1/2...)

*/

    fpGamma = 2*log(2)-beta2/12*(23+14/(gamma+2)+10/(pow(gamma+2,2))+4/(pow(gamma+2,3)));
    N = rho * NA / A; // particle g/cm3 * 1/mol * 

    dEdx = N*Z*chi*(log(E/I)+1/2*log(1+gamma/2)+fpGamma/2);

    /* for electrons */
//    double feGamma;
//    feGamma = 1 - beta2 + (gamma*gamma/8-(2*gamma+1)*log(2))/pow(1+gamma,2);
//    dEdx = N*Z*chi*(log(E/I)-fpGamma/2);

    return dEdx;
}

double Energy_Loss_Beta(double E, double th){
  double dE_n = 0,dE_tot = 0;
  int n = 100000;
  for(int j = 0; j<=n; j++){
    dE_n = Beta_Stopping_Power(E-dE_tot)*th/n;
    dE_tot += dE_n;
  }
  return dE_tot;
}


double Stopping_power_Si(double E){
  double N_A, e2, I, beta2, sp, mc2, A;
  int Z, z;
  I=173e-6;//MeV for Si
  z=1; //proton
  Z=14;
  A=28.04;//g/mol
  N_A=6.023e23;//1/mol
  e2=1.44e-13;   //(MeV*cm)
//  mc2=3727.379;     //MeV
  mc2=938.27;     //MeV
  beta2=1-pow(mc2/(E+mc2),2);
  sp = 16*M_PI*N_A*e2*e2/(0.511*beta2)*Z/A*(log(2*beta2*0.511/I/(1-beta2))-beta2);
  return sp;
}

double Stopping_power_CH2(double E){
  double N_A, e2, I, beta2, sp, mc2, A;
  int Z, z;
  I=53.4e-6;//MeV for Si
  z=13; //al 26
  Z=7;
  A=14;//g/mol
  N_A=6.023e23;//1/mol
  e2=1.44e-13;   //(MeV*cm)
//  mc2=3727.379;     //MeV
  mc2=938.27;     //MeV
  beta2=1-pow(mc2/(E+mc2),2);
  sp = 16*M_PI*N_A*e2*e2/(0.511*beta2)*Z/A*(log(2*beta2*0.511/I/(1-beta2))-beta2);
  return sp;
}

double Energy_Loss_Si(double E, double th){
  double dE_n = 0,dE_tot = 0;
  int n=100000, j;
  th *= 2.3290*1.0e-4;
  for(j = 0; j<=n; j++){
    dE_n = Stopping_power_Si(E-dE_tot)*th/n;
    dE_tot += dE_n;
  }
  return dE_tot>0
      ? dE_tot
      : 0;
}

double Energy_Loss_CH2(double E, double th){
  double dE_n = 0,dE_tot = 0;
  int n=1000000, j;
  th *= 0.91*1.0e-4;
  for(j = 0; j<=n; j++){
    dE_n = Stopping_power_CH2(E-dE_tot)*th/n;
    dE_tot += dE_n;
  }
  return dE_tot>0
      ? dE_tot
      : 0;
}
