//################################
//#
//# File : BeamSpread
//#   created on 2017/11/27 20:31:09
//#   last modified 2017/11/27 20:32:09
//#
//# 
//#


#include "TCrabatHistManager.h"

void TCrabatHistManager::BeamSpread(TH2D *hist){

  if(det.fCr[0]){
    det.PPACcalib();
    hist->Fill(180/M_PI*asin(det.Vinc[0]),180/M_PI*asin(det.Vinc[1]));
  }
}
