//################################
//#
//# File : RF1
//#   created on 2017/11/30 13:53:10
//#   
//#
//# 
//#


#include "TCrabatHistManager.h"

void TCrabatHistManager::RF1(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(det.fRf[0]);

}
