#ifndef TDETECTORS_H
#define TDETECTORS_H

#include <iostream>
#include "TApplication.h"
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <PPACcalib.h>
#include <HFiles.h>

class TDetectors{

    private:
        Int_t runNum;
        const Float_t PPACab = 236.75;

        Bool_t PsdIsHit[2][2];
        Bool_t PpacCalibIsDone;
        Bool_t PsdFlagIsDone;
        Bool_t SsdFlagIsDone;

    public:
        TDetectors(){};
        TDetectors(Int_t N);
        ~TDetectors();

        void Init();

        void SetRunNumber(Int_t N){
          runNum = N;
        }

        /* Definition */
        /* PSD0 = 0 deg */
        /* PSD1 = 20 deg */
        /* PSD2 = -20 deg but channel problem and not used */

        Bool_t fCr[3];
        Bool_t fPsdIsHit[2];
        Bool_t fSsdIsHit[15];
//       Float_t fCr[16];
        Float_t fRf[2];
        Float_t fNstdc;
        Float_t fPpac[3][5];
        Float_t fPsdE[3][32];
        Float_t fSsdE[15];
        Float_t fSsdT[15];
        Float_t fNaiE[10];

        /* Detector defined from tree */



        Float_t fPpacX[3];
        Float_t fPpacY[3];
        Float_t fTargetX, fTargetY;

        inline void PPACcalib(){

          if(PpacCalibIsDone)
            return;

            for(Int_t i=0; i<3; i++){
                if(fPpac[i][0]<200&&fPpac[i][0]>0&&fPpac[i][1]<200&&fPpac[i][1]>0)
                    fPpacX[i] = (fPpac[i][0]-fPpac[i][1]-PpacOffset(i,0) - PpacOffsetLine(i,0)) * PpacPositionGain(i,0) - PpacOffsetGeometry(i,0);
                if(fPpac[i][2]<200&&fPpac[i][2]>0&&fPpac[i][3]<200&&fPpac[i][3]>0)
                    fPpacY[i] = (fPpac[i][2]-fPpac[i][3]-PpacOffset(i,1) - PpacOffsetLine(i,1)) * PpacPositionGain(i,1) - PpacOffsetGeometry(i,1);
            }
            if(runNum>163&&runNum<174)
                fPpacX[1]+=25.0;
            if(runNum>173)
                fPpacX[1]+=25.2;

            fTargetX = fPpacX[0] - (fPpacX[0] - fPpacX[1])*PPACab/454.0;
            fTargetY = fPpacY[0] - (fPpacY[0] - fPpacY[1])*PPACab/454.0;

            VincN = sqrt(pow(fPpacX[0] - fPpacX[1],2)+pow(fPpacY[0] - fPpacY[1],2)+pow(PPACab,2));
            Vinc[0] = (fPpacX[0] - fPpacX[1])/VincN;
            Vinc[1] = (fPpacY[0] - fPpacY[1])/VincN;
            Vinc[2] = PPACab/VincN;

            PpacCalibIsDone=true;
        }

        /* calculation for PSD */

        Int_t stripX[2];
        Int_t stripY[2];

        Float_t fPsdDep[2]; //should be calibrated like 
        void PsdFlag();
        void SsdFlag();

        Float_t fPsdX[2],fPsdY[2];
        

        Float_t Vrec[3];
        Float_t VrecN;

        Float_t Theta_lab;
        Float_t Theta_lab_sigma;

        Float_t Vinc[3];
        Float_t VincN;

        Bool_t Angle_calculate();


        void SetDetectors(TTree *tree);

};

#endif //DETECTORS_H
