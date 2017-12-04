//################################
//#
//# File : Tel1_Energy
//#   created on 2017/11/30 13:27:55
//#   
//#
//# 
//#


#include "TCrabatHistManager.h"

void TCrabatHistManager::Tel1_Energy(TH1D *hist){

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
        hist->Fill(E+dE);
      }
      
    }
  }

}