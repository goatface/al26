//################################
//#
//# File : src/TCrabatHistManager
//#   created on 2017/11/28 04:16:27
//#   last modified 2017/11/28 04:16:31
//#
//# 
//#
#include "TCrabatHistManager.h"

TCrabatHistManager::TCrabatHistManager()
{

}

TCrabatHistManager::TCrabatHistManager(Int_t n) : TCrabat(n)
{

}

TCrabatHistManager::~TCrabatHistManager()
{

}

void TCrabatHistManager::Set(){

  HFiles file("./bin/list/set.list");
  vector<string> id_str = file.ReadID(); 

  for(int i = 0; i < id_str.size(); i++){
    Short_t ID = atoi(id_str.at(i).c_str());
    vector<string> setup = file.ReadSetup(i); 
    vector<string> axes = file.ReadAxes(i); 
    int dim = file.ReadDim(i); 

      if(dim==1){
        TH1D* hist1D = new TH1D(setup.at(0).c_str(),setup.at(1).c_str(),atoi(setup.at(2).c_str()),atof(setup.at(3).c_str()),atof(setup.at(4).c_str())); 
        hist1D->GetXaxis()->SetTitle(axes.at(0).c_str());
        hist1D->GetYaxis()->SetTitle(axes.at(1).c_str());
        fHist1D.push_back(hist1D);
        fHistID.push_back(ID);
        fHistDim.push_back(Dimension1);
      }
      else if(dim==2){
        TH2D* hist2D = new TH2D(setup.at(0).c_str(),setup.at(1).c_str(),atoi(setup.at(2).c_str()),atof(setup.at(3).c_str()),atof(setup.at(4).c_str()),atoi(setup.at(5).c_str()),atof(setup.at(6).c_str()),atof(setup.at(7).c_str())); 
        hist2D->GetXaxis()->SetTitle(axes.at(0).c_str());
        hist2D->GetYaxis()->SetTitle(axes.at(1).c_str());
        fHist2D.push_back(hist2D);
        fHistID.push_back(ID);
        fHistDim.push_back(Dimension2);
      }
       else if(dim==3){
        TH3D* hist3D = new TH3D(setup.at(0).c_str(),setup.at(1).c_str(),atoi(setup.at(2).c_str()),atof(setup.at(3).c_str()),atof(setup.at(4).c_str()),atoi(setup.at(5).c_str()),atof(setup.at(6).c_str()),atof(setup.at(7).c_str()),atoi(setup.at(8).c_str()),atof(setup.at(9).c_str()),atof(setup.at(10).c_str())); 
        hist3D->GetXaxis()->SetTitle(axes.at(0).c_str());
        hist3D->GetYaxis()->SetTitle(axes.at(1).c_str());
        hist3D->GetZaxis()->SetTitle(axes.at(2).c_str());
        fHist3D.push_back(hist3D);
        fHistID.push_back(ID);
        fHistDim.push_back(Dimension3);
      }
      else
        cerr << "Invalid dimension loaded" << endl;
    }
}
  


void TCrabatHistManager::loop(){

  for(Int_t i=0; i<fEntry; i++){
    fTree->GetEntry(i,0);


    Long_t l = 0, m = 0, n = 0;

    for(Int_t itr=0; itr<fHistDim.size(); itr++){

      det.Init();

      switch(fHistDim.at(itr)){

        case Dimension1 :
          if(!Assignment(fHist1D.at(l),fHistID.at(itr)))
            continue;
          l++;
          break;

        case Dimension2 :
          if(!Assignment(fHist2D.at(m),fHistID.at(itr)))
            continue;
          m++;
          break;

        case Dimension3 :
          if(!Assignment(fHist3D.at(n),fHistID.at(itr)))
            continue;
          n++;
          break;
        default :
            break;
      }
    }
  }
} 


Bool_t TCrabatHistManager::Assignment(TH1D* hist, Int_t histID)
{
  switch(histID){
    case 0 :    // {o}LOOP_Tel2_Energy{o}
      Tel2_Energy(hist);   // {o}LOOP_Tel2_Energy{o}
      break;          // {o}LOOP_Tel2_Energy{o}
    case 2 :    // {o}NaI1{o}
      NaI1(hist);   // {o}NaI1{o}
      break;          // {o}NaI1{o}
    case 4 :    // {o}Angle_recoil{o}
      Angle_recoil(hist);   // {o}Angle_recoil{o}
      break;          // {o}Angle_recoil{o}
    case 5 :    // {o}NaI2{o}
      NaI2(hist);   // {o}NaI2{o}
      break;          // {o}NaI2{o}
    case 6 :    // {o}NaI3{o}
      NaI3(hist);   // {o}NaI3{o}
      break;          // {o}NaI3{o}
    case 7 :    // {o}NaI4{o}
      NaI4(hist);   // {o}NaI4{o}
      break;          // {o}NaI4{o}
    case 8 :    // {o}NaI5{o}
      NaI5(hist);   // {o}NaI5{o}
      break;          // {o}NaI5{o}
    case 9 :    // {o}NaI6{o}
      NaI6(hist);   // {o}NaI6{o}
      break;          // {o}NaI6{o}
    case 10 :    // {o}NaI7{o}
      NaI7(hist);   // {o}NaI7{o}
      break;          // {o}NaI7{o}
    case 11 :    // {o}NaI8{o}
      NaI8(hist);   // {o}NaI8{o}
      break;          // {o}NaI8{o}
    case 12 :    // {o}NaI9{o}
      NaI9(hist);   // {o}NaI9{o}
      break;          // {o}NaI9{o}
    case 13 :    // {o}NaI10{o}
      NaI10(hist);   // {o}NaI10{o}
      break;          // {o}NaI10{o}
    case 14 :    // {o}Tel1_Energy{o}
      Tel1_Energy(hist);   // {o}Tel1_Energy{o}
      break;          // {o}Tel1_Energy{o}
    case 15 :    // {o}RF1{o}
      RF1(hist);   // {o}RF1{o}
      break;          // {o}RF1{o}
    case 16 :    // {o}RF2{o}
      RF2(hist);   // {o}RF2{o}
      break;          // {o}RF2{o}
  /* ADDHIST_SRC1 created by command "addhist" */
    default :
      return false;
      break;
  }
  
  return true;

}

Bool_t TCrabatHistManager::Assignment(TH2D* hist, Int_t histID)
{

  switch(histID){
    case 1 :    // {o}Tel1_dEvsE{o}
      Tel1_dEvsE(hist);   // {o}Tel1_dEvsE{o}
      break;          // {o}Tel1_dEvsE{o}
    case 3 :    // {o}BeamSpread{o}
      BeamSpread(hist);   // {o}BeamSpread{o}
      break;          // {o}BeamSpread{o}
 /* ADDHIST_SRC2 created by command "addhist" */
    default :
      return false;
      break;
  }
 
  return true;

}

Bool_t TCrabatHistManager::Assignment(TH3D* hist, Int_t histID)
{

  switch(histID){
 /* ADDHIST_SRC3 created by command "addhist" */
    default :
      return false;
      break;
  }
 
 return true;

} 
