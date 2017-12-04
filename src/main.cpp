#include <iostream>
#include "TApplication.h"
#include <math.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TCutG.h>
#include <TLine.h>
#include <TTree.h>
#include <TLatex.h>
#include <TRandom.h>
#include <TF1.h>
#include <TMultiGraph.h>
#include <string>
#include <vector>

#include "TCrabat.h"
#include "TCrabatHistManager.h"
#include "HFiles.h"
#include "TCalibration.h"

using namespace::std;
Int_t main(Int_t argc, Char_t **argv){

  Int_t RunN = atoi(argv[1]);;

  TCrabatHistManager Hist(RunN);
  Hist.Set();
  Hist.loop();
  Hist.Write();

/* For TCalibration, naively use these below */

  //TFile* File = new TFile(Form("rawdata/run%d_raw.root",RunN));
  //TTree* tree = (TTree*)File->Get("riken_exp");
  //TCalibration calib;
  //calib.PSDcalib(tree, RunN, 1);

  return 0;
}
