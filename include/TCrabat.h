#ifndef TCRABAT_H
#define TCRABAT_H

#include <iostream>
#include <math.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TSpectrum.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TLine.h>
#include <TRandom.h>
#include <TGraph.h>
#include <TCutG.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "TDetectors.h"
#include "TCalibration.h"
#include "HFiles.h"
#include "Functions.h"

class TCrabat{

  protected:
    TTree* fTree;
    Int_t fEntry;
    TCutG* RFGate;
    TCutG* ProtonGate;
    Int_t fRunNum;
    TCalibration* Psd_calib;
    TCalibration* Ssd_calib;
    TCalibration* NaI_calib;
    TDetectors det; 
    TFile* output;

  private:

     
  public:
    TCrabat();
    TCrabat(Int_t n);

    ~TCrabat();

    inline void SetTree(TTree* tree){
      cout << "Tree set" << endl;
      fTree = tree;
      fEntry = fTree->GetEntries();
    }

    inline void Write(){
      cout << "writing..." << endl;
      output->Write();
      output->Close();
    }


};

#endif //HISTOGRAMS_H
