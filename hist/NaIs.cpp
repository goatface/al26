//################################
//#
//# File : NaIs
//#   created on 2017/11/30 11:59:00
//#   last modified 2017/11/30 11:59:09
//#
//# Free from grandmother.
//#

#include "TCrabatHistManager.h"

void TCrabatHistManager::NaI1(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[0],0));
}

void TCrabatHistManager::NaI2(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[1],1));
}
void TCrabatHistManager::NaI3(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[2],2));
}
void TCrabatHistManager::NaI4(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[3],3));
}
void TCrabatHistManager::NaI5(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[4],4));
}
void TCrabatHistManager::NaI6(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[5],5));
}
void TCrabatHistManager::NaI7(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[6],6));
}
void TCrabatHistManager::NaI8(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[7],7));
}
void TCrabatHistManager::NaI9(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[8],8));
}
void TCrabatHistManager::NaI10(TH1D *hist){

  if(det.fCr[0])
    hist->Fill(NaI_calib->NaIEdep(det.fNaiE[9],9));
}
