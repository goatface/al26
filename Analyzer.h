//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Jan 24 13:16:34 2017 by ROOT version 5.34/18
// from TTree riken_exp/experiment tree
// found on file: /home/daid/data/al26_july16_root_new/run79_raw.root
//////////////////////////////////////////////////////////

#ifndef Analyzer_h
#define Analyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
// some may be unecessary...
#include <iostream>
#include <TStyle.h>
#include <TCanvas.h>
#include <stdio.h>
#include <TF1.h>
#include <TMath.h>
#include <TCutG.h>
#include <TGraph.h>
#include <vector>
using std::vector;

using namespace std;

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class Analyzer {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         fNstdc;
   Float_t         fRf[2];
   Bool_t          fCr[3];
   Float_t         fNaiE[10];
   Float_t         fNaiT[10];
   Float_t         fPpac[3][5];
   Float_t         fPsdE[3][32];
   Float_t         fPsdT[3][32];
   Float_t         fSsdE[15];
   Float_t         fSsdT[15];

// analysis part (not in raw data tree)
// from 26Al pre-array
// TODO: Check and correct names 25 Jan 2017 17:13:49    
   const int SsdN = 15;
   const int PsdN = 3; //  

   Bool_t 	   PpacIsHit[3];
   Bool_t 	   SsdIsHit[15];
   Bool_t 	   TelescopeIsHit[2][3];
   Double_t PpacX[3], PpacY[3];
   Double_t TargetX, TargetY;
   TF1 *fTargetX ;
   TF1 *fTargetY ;

//	Double_t			PSD0_X_Sum;
//	Double_t			PSD0_Y_Sum;
//	Double_t			PSD0_Calib[32];
   Double_t PsdE_XY_cal[3][2][16];
   Double_t PsdE_XY_max[3][2];
   Int_t PsdEmax_ch[3][2];
   Double_t PsdEcal[3];
   Double_t PsdTcal[3];
   Double_t PsdEmax[3];
   bool PsdIsHit[3][2];
  
   float Rfcal[2];
   Double_t SsdEcal[15];

   Double_t        tof[3];


   // List of branches
   TBranch        *b_fNstdc;   //!
   TBranch        *b_fRf;   //!
   TBranch        *b_fCr;   //!
   TBranch        *b_fNaiE;   //!
   TBranch        *b_fNaiT;   //!
   TBranch        *b_fPpac;   //!
   TBranch        *b_fPsdE;   //!
   TBranch        *b_fPsdT;   //!
   TBranch        *b_fSsdE;   //!
   TBranch        *b_fSsdT;   //!

   Analyzer(TChain *tree=0);
   //Analyzer(TTree *tree=0);
   virtual ~Analyzer();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   //virtual void     Init(TTree *tree);
   void     Init(TChain *tree);
   virtual UShort_t SetPpacSepZ();
   virtual Bool_t WindowCut(Double_t x=99., Double_t y=99.);
   virtual void     Loop(Int_t run=0,
     Bool_t flag_raw=0, Bool_t flag_detail=0,
     Bool_t flag_ssd=0, Bool_t flag_ppac=0, Bool_t flag_nai=0);
   virtual UShort_t PpacXYCalib(Bool_t flag_detail=0, Bool_t flag_ppac=0, Int_t run=0);
//   virtual Float_t RfShift(Int_t run=-1, Int_t rf=-1);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Analyzer_cxx
Analyzer::Analyzer(TChain *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
/*      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/home/daid/data/al26_july16_root_new/run79_raw.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/home/daid/data/al26_july16_root_new/run79_raw.root");
      }
      f->GetObject("riken_exp",tree);
*/
     return;
   }
   Init(tree);
}

Analyzer::~Analyzer()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Analyzer::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Analyzer::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Analyzer::Init(TChain *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fNstdc", &fNstdc, &b_fNstdc);
   fChain->SetBranchAddress("fRf", fRf, &b_fRf);
   fChain->SetBranchAddress("fCr", fCr, &b_fCr);
   fChain->SetBranchAddress("fNaiE", fNaiE, &b_fNaiE);
   fChain->SetBranchAddress("fNaiT", fNaiT, &b_fNaiT);
   fChain->SetBranchAddress("fPpac", fPpac, &b_fPpac);
   fChain->SetBranchAddress("fPsdE", fPsdE, &b_fPsdE);
   fChain->SetBranchAddress("fPsdT", fPsdT, &b_fPsdT);
   fChain->SetBranchAddress("fSsdE", fSsdE, &b_fSsdE);
   fChain->SetBranchAddress("fSsdT", fSsdT, &b_fSsdT);
   Notify();
}

Bool_t Analyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Analyzer::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Analyzer::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Analyzer_cxx
