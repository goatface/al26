#include "fit.h"

//void fit::FitGauss2(TH1F *histo, Float_t xMin=-10.0, Float_t xMax=-10.0, TString sgnl="std", TString clsFNC="new")
void fit::FitGauss2(TH1F *histo)
{
    Float_t xMin=0,xMax=1000;
    TString clsFNC="new";
    TString sgnl="std";
    Float_t gConst = TMath::Sqrt(2.0*TMath::Pi());

    //  By default clear the pad from the previously drawn functions.
    if (clsFNC=="new")
    {    
        TF1 *tmp;                
        tmp = (TF1*)gROOT->GetListOfFunctions()->FindObject("grf"); if(tmp) tmp->Delete();
        tmp = (TF1*)gROOT->GetListOfFunctions()->FindObject("sgf"); if(tmp) tmp->Delete();
        tmp = (TF1*)gROOT->GetListOfFunctions()->FindObject("bgf"); if(tmp) tmp->Delete(); 
               
        tmp = (TF1*)histo->GetListOfFunctions()->FindObject("sf");  if(tmp) histo->GetListOfFunctions()->Remove(tmp);
        tmp = (TF1*)histo->GetListOfFunctions()->FindObject("bf");  if(tmp) histo->GetListOfFunctions()->Remove(tmp);   
        tmp = (TF1*)histo->GetListOfFunctions()->FindObject("f");   if(tmp) histo->GetListOfFunctions()->Remove(tmp);
          
        gPad->Update();
    }
    
    //  Initialize the parameters.
    Double_t ingr[3] = {1.0, 0.0, 1.0};
    Double_t pars[8] = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0};    //  First 4 params are for sgf, next 4 for bgf.

    //  Get bin width for the histo.
    Float_t BinWidth = histo->GetBinWidth(1);
    
    //  If no limits provided use mean +/- wFit*sigma window.
    Float_t wFit = 4.0;
//     if (xMin<0.0) { xMin = ingr[1]-wFit*ingr[2]; }
//     if (xMax<0.0) { xMax = ingr[1]+wFit*ingr[2]; }

    // Show visually the range of selection.
    Float_t yCenter = 0.5*(histo->GetMaximum()+histo->GetMinimum());
    Float_t yWindow = fabs(histo->GetMaximum()-histo->GetMinimum());
    Float_t yFactor = 0.9;    
    Float_t yMin = yCenter-0.5*yWindow*yFactor;
    Float_t yMax = yCenter+0.5*yWindow*yFactor;
    TLine *tMin = new TLine(xMin,yMin,xMin,yMax); tMin->SetLineColor(30); tMin->Draw();
    TLine *tMax = new TLine(xMax,yMin,xMax,yMax); tMax->SetLineColor(30); tMax->Draw();
    gPad->Update();
    
    //  Get an estimate of the backgroung visually.
    TGraph *gr =(TGraph*)gPad->WaitPrimitive("Graph","PolyLine");

    //  Remove the fit window.
    tMin->Delete();
    tMax->Delete();

    //  Define the function to be used to fit the graph.    
    TF1 *grf = new TF1("grf","gaus");

    gr->Fit(grf,"QL");
    gr->GetFunction("grf")->GetParameters(&ingr[0]);
    gr->Delete();

    Float_t grfIntgrl = grf->Integral(xMin,xMax);    
    Float_t grfEstEvents = grfIntgrl/BinWidth;

    //  Get an estimate of the events in the signal function to help MINUIT.
    Int_t binxMin = histo->GetXaxis()->FindBin(xMin);
    Int_t binxMax = histo->GetXaxis()->FindBin(xMax);
    Float_t sfEstEvents = histo->Integral(binxMin,binxMax)-grfEstEvents;
    
    //  Set the initial parameters to help MINUIT.
    pars[0] = ((sfEstEvents*BinWidth)/gConst)/ingr[2];
    pars[1] = ingr[1];
    pars[2] = ingr[2];

    pars[4] = ingr[0];
    pars[5] = ingr[1];
    pars[6] = ingr[2];
        
    //  Signal and background (skewed gaussians with 4 parameters)
    //  For gaussian fits one parameter (par #3) is set to zero.
    TF1 *sgf = new TF1("sgf","[0]*exp(-0.5*pow((x-[1])/([2]+(x<[1])*[3]*(x-[1])),2))");    
    TF1 *bgf = new TF1("bgf","[0]*exp(-0.5*pow((x-[1])/([2]+(x<[1])*[3]*(x-[1])),2))");

    //  Using the backgroung estimate fit the Z peak.
    TF1 *f = new TF1("f","sgf+bgf");
    f->SetParNames("C1","Mean1","Sigma1","Par1","C2","Mean2","Sigma2","Par2");
    f->SetParameters(pars);
    if (sgnl=="std") {f->FixParameter(3,0.0); f->FixParameter(7,0.0);}  //  Fit with normal gaussians
    histo->Fit(f,"L","",xMin,xMax);
    f->GetParameters(pars);

    //  Deside the colors so that "signal" is always blue, and "background" is red.
    //  Also set the names appropriately.
    Int_t sflC, sflS, bflC, bflS;
    if (pars[0]>pars[4]) {sflC=154; sflS=2; bflC=2; bflS=4;}
    else                 {bflC=154; bflS=2; sflC=2; sflS=4;}
    
    //  Draw the signal     
    TF1 *sf = new TF1("sf","sgf",xMin,xMax);   
    sf->SetParameters(&pars[0]);    
    sf->SetLineColor(sflC);    sf->SetLineStyle(sflS);
    sf->Draw("same");
    Float_t sfIntgrl = sf->Integral(xMin,xMax);    
    Float_t sfEvents = sfIntgrl/BinWidth;

    //  Draw the backgroung.
    TF1 *bf = new TF1("bf","bgf",xMin,xMax);        
    bf->SetParameters(&pars[4]);   
    bf->SetLineColor(bflC);    bf->SetLineStyle(bflS);
    bf->Draw("same");
    Float_t bfIntgrl = bf->Integral(xMin,xMax);    
    Float_t bfEvents = bfIntgrl/BinWidth;

    //  Add the functions to the histogram.
    histo->GetListOfFunctions()->Add(sf);
    histo->GetListOfFunctions()->Add(bf);

    //  Set the colors on the stats box. This works with ROOT version 4.0
    gPad->Update();
    TPaveStats *pave = (TPaveStats*)histo->GetListOfFunctions()->FindObject("stats");  
    histo->GetListOfFunctions()->Remove(pave);
    histo->SetStats(0);
    pave->GetLineWith("C1")->SetTextColor(sflC);
    pave->GetLineWith("Mean1")->SetTextColor(sflC);
    pave->GetLineWith("Sigma1")->SetTextColor(sflC);
    pave->GetLineWith("Par1")->SetTextColor(sflC);  
    pave->GetLineWith("C2")->SetTextColor(bflC);
    pave->GetLineWith("Mean2")->SetTextColor(bflC);
    pave->GetLineWith("Sigma2")->SetTextColor(bflC);
    pave->GetLineWith("Par2")->SetTextColor(bflC);
    pave->Draw();
       
    return;
}

void fit::FitGauss(TH1D* hist){

//  TF1 *tmp;                
//  tmp = (TF1*)gROOT->GetListOfFunctions()->FindObject("grf"); if(tmp) tmp->Delete();
//  tmp = (TF1*)gROOT->GetListOfFunctions()->FindObject("sgf"); if(tmp) tmp->Delete();
//  tmp = (TF1*)gROOT->GetListOfFunctions()->FindObject("bgf"); if(tmp) tmp->Delete(); 
//         
//  tmp = (TF1*)histo->GetListOfFunctions()->FindObject("sf");  if(tmp) histo->GetListOfFunctions()->Remove(tmp);
//  tmp = (TF1*)histo->GetListOfFunctions()->FindObject("bf");  if(tmp) histo->GetListOfFunctions()->Remove(tmp);   
//  tmp = (TF1*)histo->GetListOfFunctions()->FindObject("f");   if(tmp) histo->GetListOfFunctions()->Remove(tmp);
    
//  gPad->Update();
 
  Double_t xmin=400.0, xmax=600.0;
  Double_t ymin, ymax;
  Int_t binxMin = hist->GetXaxis()->FindBin(xmin);
  Int_t binxMax = hist->GetXaxis()->FindBin(xmax);

  ymin = hist->GetBinContent(binxMin);
  ymax = hist->GetBinContent(binxMax);

  Double_t peak_rough  = (Double_t)hist->GetBinContent((binxMax-binxMin)/2);
  
  Double_t pars_first[5] = {(ymax-ymin)/(xmax-xmin), ymax-(ymax-ymin)/(xmax-xmin)*xmax, peak_rough - (ymax+ymin)/2, (xmin+xmax)/2 , (xmax-xmin)*0.68};
  TF1 *bf  = new TF1("bf","[0]*x+[1]");
  TF1 *grf = new TF1("grf","[0]*exp(-0.5*pow((x-[1])/[2],2))");
  TF1 *sf  = new TF1("sf","bf+grf");
  sf->SetParameters(pars_first);
  hist->Fit(sf,"L","",xmin,xmax);

//
//  Double_t pars[8] = {1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0};    //  First 4 params are for sgf, next 4 for bgf.
//  TF1 *sgf = new TF1("sgf","[0]*exp(-0.5*pow((x-[1])/([2]+(x<[1])*[3]*(x-[1])),2))");    
//  TF1 *bgf = new TF1("bgf","[0]*exp(-0.5*pow((x-[1])/([2]+(x<[1])*[3]*(x-[1])),2))");
//  TF1 *f = new TF1("f","sgf+bgf");
//  f->SetParNames("C1","Mean1","Sigma1","Par1","C2","Mean2","Sigma2","Par2");
//  f->SetParameters(pars);
//
  return;

}
