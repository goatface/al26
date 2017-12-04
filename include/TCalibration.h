#ifndef TCALIBRATION_H
#define TCALIBRATION_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <TROOT.h>
#include <TTree.h>
#include <TRandom.h>
#include <TH1.h>
#include <TF1.h>
#include <string>
#include <vector>
#include <TGraph.h>
#include <TLatex.h>
#include <TSpectrum.h>
#include <TCanvas.h>

#include <HFiles.h>
#include <TDetectors.h>

using namespace::std;

class TCalibration{

    private:
        Float_t SSDgain[16], SSDoffset[16], PSDoffset[3][32], PSDgain[3][32], NaIgain[10], NaIoffset[10], NaIGainShift[10];
        Float_t SsdEth[16];

        Double_t CalibSigma;
        Double_t GainCalib, OffsetCalib;

        void Alpha3Calib(TH1D* hist, Int_t bin, Int_t hMin, Int_t hMax);
        void LSM(Int_t N, Double_t *u, Double_t *v, Double_t *a, Double_t *b, Double_t *fSigma);
 
        void PSDcalib_write(double* gain, double* offset, int psd);
        void SSDcalib_write(double gain, double offset, int ssd);

    public:
        TCalibration(){};
        TCalibration(string Detector, int runNum);
        ~TCalibration();

        Float_t SSDEdep(Float_t RawData, Int_t fSsdN){
            Float_t dE = gRandom->Uniform(0.0,1.0);
            Float_t E = SSDgain[fSsdN]*(RawData + dE + SSDoffset[fSsdN]);
            return E>SsdEth[fSsdN]
                ? E
                : 0;
        }
        Float_t PSDEdep(Float_t RawData, Int_t fPsdN, Int_t fPsdCh){
            Double_t dE = gRandom->Uniform(0.0,1.0);
            Float_t E = PSDgain[fPsdN][fPsdCh]*(RawData + dE + PSDoffset[fPsdN][fPsdCh]);
            return E > 0
                ? E
                : 0;
        }
        Float_t NaIEdep(Float_t RawData, Int_t fNaiN){
            Double_t dE = gRandom->Uniform(0.0,1.0);
            return NaIgain[fNaiN]*(RawData + dE + NaIoffset[fNaiN])*NaIGainShift[fNaiN];
        }

        void SSDcalib(TTree* fTree, Int_t runNum ,Int_t fSsdN){

            TDetectors* det = new TDetectors(runNum);
            det->SetDetectors(fTree);
            Int_t n = fTree->GetEntries();

            Int_t bin=500;
            Double_t hMin=0,hMax=4096;
            Double_t Eth=400;

            TH1D* hist = new TH1D("Hist","Hist",bin,hMin,hMax);

            for(Int_t j=0; j<n; j++){
                fTree->GetEntry(j,0);
                if(det->fSsdE[fSsdN]>Eth)
                    hist->Fill(det->fSsdE[fSsdN]);
            }

            Alpha3Calib(hist, bin, hMin, hMax);
            SSDcalib_write(GainCalib, OffsetCalib,fSsdN);

        }


        void PSDcalib(TTree* fTree, Int_t runNum, Int_t fPsdN){

            TDetectors* det = new TDetectors(runNum);
            det->SetDetectors(fTree);
            Int_t n = fTree->GetEntries();

            Double_t gain[32],offset[32];
            Int_t bin=80;
            Double_t hMin=0,hMax=4096;
            Double_t Eth=500;

            TH1D* hist[32];
            for(Int_t fPsdCh=0; fPsdCh<32; fPsdCh++){
                hist[fPsdCh] = new TH1D("TCalibration","TCalibration", bin, hMin, hMax);
                for(Int_t i=0; i<n; i++){
                    fTree->GetEntry(i,0);
                    if(det->fPsdE[fPsdN][fPsdCh]>Eth)
                        hist[fPsdCh]->Fill(det->fPsdE[fPsdN][fPsdCh]);
                }
                Alpha3Calib(hist[fPsdCh], bin, hMin, hMax);
                gain[fPsdCh] = GainCalib;
                offset[fPsdCh] = OffsetCalib;
            }

            PSDcalib_write(gain,offset,fPsdN);

        };

};

#endif //CALIBRATION_H
