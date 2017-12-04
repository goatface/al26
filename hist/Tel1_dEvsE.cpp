//################################
//#
//# File : Tel1_dEvsE
//#   created on _DATE_
//#   last modified 2017/11/30 18:18:21
//#
//# 
//#


#include "TCrabatHistManager.h"

void TCrabatHistManager::Tel1_dEvsE(TH2D *hist){

  if(det.fCr[0]){
    det.PsdFlag();
    det.SsdFlag();
    if(det.fPsdIsHit[0]){
      Float_t dE = Psd_calib->PSDEdep(det.fPsdDep[0],2,det.stripX[0]);
      if(det.fSsdIsHit[5]){
        Float_t E = Ssd_calib->SSDEdep(det.fSsdE[5],5);
        if(det.fSsdIsHit[6])
          E += Ssd_calib->SSDEdep(det.fSsdE[6],6);
        if(det.fSsdIsHit[1])
          E += Ssd_calib->SSDEdep(det.fSsdE[1],1);
        hist->Fill(dE,E+dE);
      }
      
    }
  }
}
