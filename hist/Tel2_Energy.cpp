//################################
//#
//# File : Tel2_Energy
//#   created on _DATE_
//#   last modified 2017/11/26 22:52:38
//#
//# 
//#


#include "TCrabatHistManager.h"

void TCrabatHistManager::Tel2_Energy(TH1D *hist){


  if(det.fCr[0]){
    det.PsdFlag();
    det.SsdFlag();
    if(det.fPsdIsHit[1]){
      Float_t dE = Psd_calib->PSDEdep(det.fPsdDep[1],1,det.stripX[1]);
      if(det.fSsdIsHit[4]){
        Float_t E = Ssd_calib->SSDEdep(det.fSsdE[4],4);
        if(det.fSsdIsHit[7])
          E += Ssd_calib->SSDEdep(det.fSsdE[7],7);
        hist->Fill(E+dE);
      }
      
    }
  }


}
