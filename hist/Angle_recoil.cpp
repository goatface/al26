//################################
//#
//# File : Angle_recoil
//#   created on 2017/11/27 20:40:24
//#   last modified 2017/11/27 20:40:40
//#
//# 
//#


#include "TCrabatHistManager.h"

void TCrabatHistManager::Angle_recoil(TH1D *hist){

  det.PsdFlag();
  if(det.fCr[0]){
    if(det.Angle_calculate()){
      hist->Fill(det.Theta_lab*180.0/M_PI);
    }
  }

}
