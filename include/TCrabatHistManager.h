//################################
//#
//# File : include/TCrabatHistManager
//#   created on 2017/11/28 04:17:08
//#   last modified 2017/11/28 04:17:14
//#
//# Free from grandmother.
//#

#ifndef TCRABATHISTMANAGER_H
#define TCRABATHISTMANAGER_H
#include "TCrabat.h"

class TCrabatHistManager : public TCrabat{

  private:
    vector<TH1D*> fHist1D;
    vector<TH2D*> fHist2D;
    vector<TH3D*> fHist3D;

    enum dimension{
      Dimension1 = 1,
      Dimension2 = 2,
      Dimension3 = 3
    };

    vector<int> fHistID;
    vector<dimension> fHistDim;

     Bool_t Assignment(TH1D* hist, Int_t histID);
     Bool_t Assignment(TH2D* hist, Int_t histID);
     Bool_t Assignment(TH3D* hist, Int_t histID);

    void Tel2_Energy(TH1D *hist); // {o}INC_Tel2_Energy{o}
    void Tel1_dEvsE(TH2D *hist); // {o}Tel1_dEvsE{o}
    void NaI1(TH1D *hist); // {o}NaI1{o}
    void BeamSpread(TH2D *hist); // {o}BeamSpread{o}
    void Angle_recoil(TH1D *hist); // {o}Angle_recoil{o}
    void NaI2(TH1D *hist); // {o}NaI2{o}
    void NaI3(TH1D *hist); // {o}NaI3{o}
    void NaI4(TH1D *hist); // {o}NaI4{o}
    void NaI5(TH1D *hist); // {o}NaI5{o}
    void NaI6(TH1D *hist); // {o}NaI6{o}
    void NaI7(TH1D *hist); // {o}NaI7{o}
    void NaI8(TH1D *hist); // {o}NaI8{o}
    void NaI9(TH1D *hist); // {o}NaI9{o}
    void NaI10(TH1D *hist); // {o}NaI10{o}
    void Tel1_Energy(TH1D *hist); // {o}Tel1_Energy{o}
    void RF1(TH1D *hist); // {o}RF1{o}
    void RF2(TH1D *hist); // {o}RF2{o}
     // ADDHIST_INC

  public:
    TCrabatHistManager();
    TCrabatHistManager(Int_t n);
    ~TCrabatHistManager();
    void Set();
    void loop();


};

#endif
