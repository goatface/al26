#include <TCrabat.h>
#include <HFiles.h>
#include <stdlib.h>
#include <string>
#include <vector>


TCrabat::TCrabat()
{
}

TCrabat::TCrabat(Int_t n)
{

  cout << "Constructer loaded" << endl;
  
  fRunNum = n;
  TFile* File = new TFile(Form("rawdata/run%d_raw.root",fRunNum));
  TTree* tree = (TTree*)File->Get("riken_exp");
  SetTree(tree);
 
  output = new TFile(Form("rootfiles/%d.root",fRunNum),"recreate");
 
  det.SetRunNumber(fRunNum);
  det.SetDetectors(fTree);
 
  NaI_calib = new TCalibration("NaI", fRunNum);
  Psd_calib = new TCalibration("PSD", fRunNum);
  Ssd_calib = new TCalibration("SSD", fRunNum);

}

TCrabat::~TCrabat()
{

  delete NaI_calib;
  delete Psd_calib;
  delete Ssd_calib;

}
