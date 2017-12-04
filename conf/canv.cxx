#include "canv.h"

canv::canv(){
  
//    canvas = new TCanvas("Histgrams","Histograms",1000,1000);

    nHist=0;
    hSet1D = false; 
    hSet2D = false; 

    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat("nemrui");
    gStyle->SetTitleFont(22);
    gStyle->SetTextFont(22);
    gStyle->SetLabelFont(22);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetOptLogz();

    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
  
}


void CompareHist(TH1D* hist1, TH1D* hist2){

  hist1->SetMarkerStyle(20);
  hist1->SetMarkerSize(0.5);
  hist1->SetName(0);
  hist1->GetXaxis()->SetLabelSize(0.03);
  hist1->GetXaxis()->SetLabelFont(22);
  hist1->GetXaxis()->SetTitleFont(22);
  hist1->GetXaxis()->CenterTitle();
  hist1->GetYaxis()->SetTitleFont(22);
  hist1->GetYaxis()->SetLabelSize(0.03);
  hist1->GetYaxis()->SetLabelFont(22);
  hist1->GetYaxis()->CenterTitle();
  hist1->SetTitle(0);

  hist1->Draw();
  hist2->Draw("same");

}

/*
TH1D* canv::CombineHist(TH1D* hist1, TH1D* hist2){

  if(hist1->GetNbin()!=hist2->GetNbin()){
    cerr << " Different binning " <<endl;
    return;
  }
  TH1D* cHist = new TH1D(hist1->GetNbin(),
   hHist1D[n] = hist;

}
*/


void canv::SetHist1D(TH1D* hist){


  hHist1D[nHist] = hist;
  hHist1D[nHist]->SetMarkerStyle(20);
  hHist1D[nHist]->SetMarkerSize(0.5);
  hHist1D[nHist]->SetName(0);
  hHist1D[nHist]->SetStats(0);

  hHist1D[nHist]->GetXaxis()->SetLabelSize(0.03);
  hHist1D[nHist]->GetXaxis()->SetLabelFont(22);
  hHist1D[nHist]->GetXaxis()->SetTitleFont(22);
  hHist1D[nHist]->GetXaxis()->CenterTitle();
  hHist1D[nHist]->GetYaxis()->SetTitleFont(22);
  hHist1D[nHist]->GetYaxis()->SetLabelSize(0.03);
  hHist1D[nHist]->GetYaxis()->SetLabelFont(22);
  hHist1D[nHist]->GetYaxis()->CenterTitle();
  hHist1D[nHist]->SetTitle(0);

//    hHist[n]->Draw("AL");

  SortObject[nHist] = HIST_1D;
  nHist++;
  hSet1D = true;

}


void canv::SetHist2D(TH2D* hist){

  hHist2D[nHist] = hist;
  hHist2D[nHist]->SetMarkerStyle(20);
  hHist2D[nHist]->SetMarkerSize(0.5);
  hHist2D[nHist]->SetName(0);
  hHist2D[nHist]->SetStats(0);
  hHist2D[nHist]->GetXaxis()->SetLabelSize(0.03);
  hHist2D[nHist]->GetXaxis()->SetLabelFont(22);
  hHist2D[nHist]->GetXaxis()->SetTitleFont(22);
  hHist2D[nHist]->GetXaxis()->CenterTitle();
  hHist2D[nHist]->GetYaxis()->SetTitleFont(22);
  hHist2D[nHist]->GetYaxis()->SetLabelSize(0.03);
  hHist2D[nHist]->GetYaxis()->SetLabelFont(22);
  hHist2D[nHist]->GetYaxis()->CenterTitle();
  hHist2D[nHist]->SetTitle(0);

//    hHist[n]->Draw("AL");

  SortObject[nHist] = HIST_2D;
  nHist++;
  hSet2D = true;

}



void canv::MultiDraw(){

    cout << nHist << "histograms are set" << endl;

    cd(1);
    hHist1D[0]->Draw();


    for(Int_t i=1;i<nHist;i++){
        hHist1D[i]->SetMarkerColor(i+1);
        hHist1D[i]->Draw("same");
    }
}


void canv::DrawAll(){

  if(nHist>1){
      switch(nHist){
          case 2:
             Divide(2,1);
              break;
          case 3:
              Divide(3,1);
              break;
          case 4:
              Divide(2,2);
              break;
          case 5:
              Divide(3,2);
              break;
          case 6:
              Divide(3,2);
              break;
          case 7:
              Divide(3,3);
              break;
          case 8:
              Divide(3,3);
              break;
          case 9:
              Divide(3,3);
              break;
          default:
              cerr << "Error: Too much histgrams." << endl;
              break;
      }
        for(Int_t i=0;i<nHist;i++){
            cd(i+1);
            switch(SortObject[i]){
              case HIST_1D :
                hHist1D[i]->SetMarkerColor(i+1);
                hHist1D[i]->Draw();
                break;
              case HIST_2D :
                hHist2D[i]->SetMarkerColor(i+1);
                hHist2D[i]->Draw("colz");
                break;
              case GRAPH :
                hHist1D[i]->SetMarkerColor(i+1);
                hHist1D[i]->Draw();
                break;
            }
        }
  }else{

    switch(SortObject[0]){
      case HIST_1D :
        hHist1D[0]->SetMarkerColor(0+1);
        hHist1D[0]->Draw();
        break;
      case HIST_2D :
        hHist2D[0]->SetMarkerColor(0+1);
        hHist2D[0]->Draw("colz");
        break;
      case GRAPH :
        hHist1D[0]->SetMarkerColor(0+1);
        hHist1D[0]->Draw();
        break;
    }
 
  }

}

