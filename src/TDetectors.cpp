#include <TDetectors.h>
#include <TTree.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace::std;

TDetectors::TDetectors(Int_t N){
  runNum = N;
  for(Int_t i=0; i < 2 ; i++){
    stripX[i] = 0;
    stripY[i] = 0;
  }
}
TDetectors::~TDetectors(){
}

void TDetectors::Init(){
  PsdFlagIsDone=false;
  SsdFlagIsDone=false;
  PpacCalibIsDone=false;
}

void TDetectors::SsdFlag(){

  if(SsdFlagIsDone)
    return;

  Double_t Eth[15];
  Eth[1] = 140; 
  Eth[2] = 115; 
  Eth[3] = 90; 
  Eth[4] = 105; 
  Eth[5] = 108; 
  Eth[6] = 90; 
  Eth[7] = 90; 

  for(Int_t i=0;i<15;i++){
    if(fSsdE[i]>=Eth[i])
      fSsdIsHit[i] = true;
    else
      fSsdIsHit[i] = false;
  }

  SsdFlagIsDone=true;

}

void TDetectors::PsdFlag(){

  if(PsdFlagIsDone)
    return;

  Float_t Threshold = 300.0;

  for(Int_t i=0; i < 2 ; i++){
    stripX[i] = 0;
    stripY[i] = 16;
    fPsdIsHit[i]=false;
    for(Int_t j=0; j < 2 ; j++)
      PsdIsHit[j][i]=false;
  }


  for(Int_t i=0; i<16; i++){
    if(fPsdE[2][i] > Threshold && fPsdE[2][i] < 3841){
      if(fPsdE[2][i] >= fPsdE[2][stripX[0]])
        stripX[0] = i;
      PsdIsHit[0][0]=true;
    }
  }

  for(Int_t i=16; i<32; i++){
    if(fPsdE[2][i] > Threshold && fPsdE[2][i] < 3841){
      if(fPsdE[2][i] > fPsdE[2][stripY[0]])
        stripY[0] = i;
      PsdIsHit[0][1]=true;
      }
  }

  if(PsdIsHit[0][0]&&PsdIsHit[0][1])
    fPsdIsHit[0]=true;

  fPsdDep[0] = fPsdE[2][stripX[0]];

  fPsdX[0] = 50./17. * (- stripX[0] + 8 )-50./17./2.;
  fPsdY[0] = 50./17. * (- (stripY[0] - 16) + 8 ) - 50./17./2.;


  for(Int_t i=0; i<16; i++){
    if(fPsdE[1][i] > Threshold && fPsdE[1][i] < 3841){
      if(fPsdE[1][i] >= fPsdE[1][stripX[1]])
        stripX[1] = i;
      PsdIsHit[1][0]=true;
    }
  }
  for(Int_t i=16; i<32; i++){
    if(fPsdE[1][i] > Threshold && fPsdE[1][i] < 3841){
      if(fPsdE[1][i] > fPsdE[1][stripY[1]])
        stripY[1] = i;
      PsdIsHit[1][1]=true;
    }
  }

  if(PsdIsHit[1][0]&&PsdIsHit[1][1])
    fPsdIsHit[1]=true;

  fPsdX[1] = 50./17. * (- stripX[1] + 8 )-50./17./2.;
  fPsdY[1] = 50./17. * (- (stripY[1] - 16) + 8 ) - 50./17./2.;

  fPsdDep[1] = fPsdE[1][stripX[1]];

  PsdFlagIsDone=true;

}


Bool_t TDetectors::Angle_calculate(){

  PPACcalib();
  PsdFlag();

  Double_t L1 = 236;
  Double_t L2 = 247.7;

  Double_t phi1 = 0;
  Double_t phi2 = 18.48/180.0*M_PI;
  Double_t delta2 = 1.48/180.0*M_PI;

  if(fPsdIsHit[0]&&fPsdIsHit[1])
    return false;
  
  if(fPsdIsHit[0]){

    Vrec[0] = fTargetX + fPsdX[0];
    Vrec[1] = fTargetY - fPsdY[0];
    Vrec[2] = L1;

    VrecN = sqrt(pow(Vrec[0],2)+pow(Vrec[1],2)+pow(Vrec[2],2));

    for(Int_t i=0; i<3; i++)
    {
      Vrec[i] = Vrec[i]/VrecN;
    }

//      DeltaOmega = 1.004*1.004/pow(VrecN,2)/sqrt(1+pow(Vrec[0]/Vrec[2],2));

//    DeltaOmega = 1.004*1.004/pow(VrecN,2); //!!!! Do not forget to change it as to consider angle dependence!
    // Surface area should be multiplied by cosine

  //Theta_lab = acos(Vinc[0]*Vrec[0]+Vinc[1]*Vrec[1]+Vinc[2]*Vrec[2]);
//  Theta_lab = -1;

  }
  else if(fPsdIsHit[1])
  { 

    Double_t V_L2[3]; // Vector Center of Target to center of PSD2
    Double_t V_psd2[3]; // Vector center of psd2 to (X,Y)
    V_L2[0] = -L2*sin(phi2);
    V_L2[1] = 0;
    V_L2[2] = L2*cos(phi2);

    V_psd2[0] = fPsdX[1]*cos(phi2+delta2);
    V_psd2[1] = fPsdY[1];
    V_psd2[2] = fPsdX[1]*sin(phi2+delta2);

    Vrec[0] = V_L2[0] + V_psd2[0] - fTargetX;
    Vrec[1] = V_L2[1] + V_psd2[1] - fTargetY;
    Vrec[2] = V_L2[2] + V_psd2[2];

    VrecN = sqrt(pow(Vrec[0],2)+pow(Vrec[1],2)+pow(Vrec[2],2));

    for(Int_t i=0; i<3; i++)
    {
      Vrec[i] = Vrec[i]/VrecN;
    }

//    DeltaOmega = 1.004*1.004/pow(VrecN,2);  //!!!! Do not forget to change it as to consider angle dependence!
  }else{
    return false;
  }
    Theta_lab = acos(Vinc[0]*Vrec[0]+Vinc[1]*Vrec[1]+Vinc[2]*Vrec[2]);
    return true;

}



void TDetectors::SetDetectors(TTree *tree){


    HFiles* DataFile = new HFiles();
    DataFile->OpenFile("bin/list/SetDetector.list");

    Bool_t UseRf=false, UseCr=false, UsePpac=false, UseSsd=false, UsePsd=false, UseNaI=false, UseNstdc=false;

    if(DataFile->ReadEquality("RF")=="on")
        UseRf=true;

    if(DataFile->ReadEquality("CR")=="on")
        UseCr=true;

    if(DataFile->ReadEquality("PPAC")=="on")
        UsePpac=true;

    if(DataFile->ReadEquality("SSD")=="on")
        UseSsd=true;

    if(DataFile->ReadEquality("PSD")=="on")
        UsePsd=true;

    if(DataFile->ReadEquality("NaI")=="on")
        UseNaI=true;

    if(DataFile->ReadEquality("NSTDC")=="on")
        UseNstdc=true;
/*
    cout << "RF " << UseRf << endl;
    cout << "Cr " << UseCr << endl;
    cout << "PPAC "  << UsePpac << endl;
    cout << "SSD "<< UseSsd << endl;
    cout << "PSd "<< UsePsd << endl;
    cout << "NaI "<< UseNaI << endl;
    cout << "NSTDC "<< UseNstdc << endl;
*/

    tree->SetBranchAddress("fRf", fRf);
    tree->SetBranchAddress("fCr", fCr);
    tree->SetBranchAddress("fPpac", fPpac);
    tree->SetBranchAddress("fNstdc", &fNstdc);
    tree->SetBranchAddress("fSsdE", fSsdE);
    tree->SetBranchAddress("fSsdT", fSsdT);
    tree->SetBranchAddress("fPsdE", fPsdE);

    tree->SetBranchAddress("fNaiE", fNaiE);

    tree->SetBranchStatus("fRf", UseRf);
    tree->SetBranchStatus("fCr", UseCr);
    tree->SetBranchStatus("fPpac", UsePpac);
    tree->SetBranchStatus("fNstdc", UseNstdc);
    tree->SetBranchStatus("fSsdE", UseSsd);
    tree->SetBranchStatus("fSsdT", UseSsd);
    tree->SetBranchStatus("fPsdE", UsePsd);

    tree->SetBranchStatus("fNaiE", UseNaI);

    DataFile->CloseFile();
}
