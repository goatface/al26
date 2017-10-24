/*
 Copyright: 
 2010, 2011, 2012, 2013, 2015, 2016 daid KAHL

 This file is derived from crabat

 crabat is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 crabat is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with crabat.  If not, see <http://www.gnu.org/licenses/>.
 
*/
/**
@file run.h
@author daid
@version 1.0
@brief Global flag initalization, histogram memory control and initalization.
@details Basically allocates, initalizes and deletes memory for all histograms.
@date 09 Nov 2011 22:50:31 
*/
//Memory allocation / freeing for crabat
//This appeared to speed up processing time;
//however, no benchmarks were taken
//(previously some of this was done inline in Analyzer.cxx)
//Anyway, it's a little more modular feeling this way

#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TH3.h>


/**
@brief Prints the invocation usage to standard error.
@details The function is called whenever we get strange runtime options.\n
Prints to standard error the options accepted by the binary run.\n
*/
void Usage() {
  fprintf(stderr, "\nrun processes the data analysis\n");
  fprintf(stderr, "Must analyze at least one data type (raw and/or detailed)\n");
  fprintf(stderr, "Must analyze at least one detector type (ssd, ppac)\n");
  fprintf(stderr, "Copyright 2010-2016 daid KAHL (and others, see AUTHORS)\n");
  fprintf(stderr, "Usage: run [-R RunNumber] [-B ] [-I path] [-r ] [-d ] [-s ] [-p ] [-n]\n");
  fprintf(stderr, "Description of options:\n");
  fprintf(stderr, "  -R <int>\t: Call run on RunNumber <int> (can be entered manually at runtime also)\n");
  fprintf(stderr, "  -B\t\t: Call run in non-interactive Batch mode.  Requires -R flag.\n");
  fprintf(stderr, "  -I <string>\t: Use ROOT Tree input directory path <string> instead of reading from run.conf\n");
  fprintf(stderr, "  -r\t\t: Raw data histograms\n");
  fprintf(stderr, "  -d\t\t: Detailed data histograms\n");
  fprintf(stderr, "  -s\t\t: Analyze SSD data\n");
  fprintf(stderr, "  -p\t\t: Analyze PPAC data\n");
  fprintf(stderr, "  -n\t\t: Analyze NaI data\n");
};


// read any arguments passed to run at runtime
// Never change these here!
Bool_t flag_Run = false; // flag_Run controls automated mode (no user input)
Bool_t flag_Batch = false; // controls Batch mode - must be used with -R flag as well
Bool_t flag_Header = false; // pass a header rather than read it from run.conf
Bool_t flag_Inpath = false; // pass the ROOT Tree Input Path (directory) rather than read from run.conf
Bool_t flag_raw=false, flag_detail=false;
Bool_t flag_ssd=false, flag_ppac=false, flag_nai=false; 

// settings for number of SSD strips and telescopes
const int num_telescopes=3;
const int num_strips=96;
const int num_ssds=15; 
//Histograms
const int num_events=400;

TH1F *hRf_ds[2]; 
TH1F *hRf_ssd[2]; 
TH1F *hRfcal[2]; 

TH1F *hNstdc;
TH2F *hF2SsdRf[2];

TH1F *hTelescopeELow[num_telescopes];
TH1F *hTelescopeEHigh[num_telescopes];
TH1F *hTelescopeEHighcut[num_telescopes];

TH2F *hPsdStripE[num_telescopes] ;
TH2F *hPsdStripT[num_telescopes] ;
TH2F *hPsdStripEcal[num_telescopes] ;
TH2F *hPsdStripTcal[num_telescopes] ;
TH2F *hSsdE ;
TH2F *hSsdT ;
TH2F *hSsdEcal ;
//Each PSD channel
TH1F *strip_ch[num_strips];
TH1F *strip_ch_gated[num_strips];
TH1F *strip_cal_ch[num_strips];
//Each SSD detector
TH1F *ssd_ch[num_ssds];
TH1F *ssd_ch_gated[num_ssds];
TH1F *ssd_cal_ch[num_ssds];

//DeltaE-E
TH2F *hDE_ELow[num_telescopes];
TH2F *hDE_ELowcut[num_telescopes];
TH2F *hDE_EHigh[num_telescopes];
TH2F *hPsdXY[num_telescopes];

TH2F *hE1_E2;
TH2F *hTelescopeE_Tof[num_telescopes];
TH2F *hTelescopeE_Tofcut[num_telescopes];
TH1F *hProton[num_telescopes];
TH1F *hDeuteron[num_telescopes];

TH2F *hRF0Tof ;
TH2F *hRF1Tof ;

TH1F *hPpac0TX1 ;
TH1F *hPpac0TX2 ;
TH1F *hPpac0TY1 ;
TH1F *hPpac0TY2 ;
TH1F *hPpac1TX1 ;
TH1F *hPpac1TX2 ;
TH1F *hPpac1TY1 ;
TH1F *hPpac1TY2 ;
TH1F *hPpac2TX1 ;
TH1F *hPpac2TX2 ;
TH1F *hPpac2TY1 ;
TH1F *hPpac2TY2 ;

TH2F *hPpac0XY ;
TH2F *hPpac0XYcut ;

TH2F *hPpac1XY ;
TH2F *hPpac1XYcut ;

TH2F *hPpac2XY ;
TH2F *hPpac2XYcut ;

TH2F *hTargetXY ;
TH2F *hTargetXYcut ;
TH2F *hTargetXY_26al ;
TH2F *hTargetXYcut_26al ;

TH2F *hPpac0XRF0 ;
TH2F *hPpac0XRF0_26al ;
TH2F *hPpac0XRF0_26al_ds ;
TH2F *hPpac0XRF1_26al ;
TH2F *hPpac0XRF0ds ;
TH2F *hPpac0XRF1ds ;
TH2F *hPpac0XRF0ssd ;
TH2F *hPpac0XRF1ssd ;

TH2F *hPpac1XRF0 ;
TH2F *hPpac1XRF0_26al ;
TH2F *hPpac1XRF1_26al ;
TH2F *hPpac1XRF0ds ;
TH2F *hPpac1XRF1ds ;
TH2F *hPpac1XRF0ssd ;
TH2F *hPpac1XRF1ssd ;

TH2F *hPpac0XRF1 ;
TH2F *hPpac0XRF1cut ;
TH2F *hPpac1XRF1 ;
TH2F *hPpac1XRF1cut ;

// NaI 20 Sep 2016 11:12:29 

TH2F *hNaIE;
TH2F *hNaIEcal;
TH1F *nai_ch[10];
TH1F *nai_cal_ch[10];


/*
        TH2F * ch_str   = new TH2F("ch_str","PSD1 X Channels vs Strip",40,-1,39,500,0,10);
        TH2F * ch_str2 = new TH2F("ch_str2","PSD2 X Calib Energy vs Strip",40,-1,39,500,0,15);
        TH2F * ch_str3 = new TH2F("ch_str3","PSD2 Y Calib Energy vs Strip",40,-1,39,500,0,15);
        TH2F * dE_E     = new TH2F("dE_E","dE_E",1000,0,10,2000,0,20);
*/



/**
@brief Initalize the histograms based on which data are to be processed
@details This function must be called before any histograms are Fill'd
All memory allocation for histograms is completed previously in the run header\n
Here we define the histograms and set all relevant histogramming options.
*/
void HistInit(){ // histogram initalization
   
  char name[100];
  int xbin,ybin;
  double xmin,xmax,ymin,ymax;
  if (flag_nai) {
    if (flag_raw){
      xbin=10;xmin=-0.5;xmax=9.5;
      ybin=4096;ymin=0.;ymax=4096.;
      hNaIE = new TH2F("hNaIE","hNaIE",xbin,xmin,xmax,ybin,ymin,ymax);
      hNaIE->GetXaxis()->SetTitle("NaI number");
      hNaIE->GetYaxis()->SetTitle("Energy (ch)");
      hNaIE->SetOption("COL");
      for (UShort_t i=0;i<10;i++) {
        sprintf(name,"nai_ch%d",i);
        nai_ch[i]= new TH1F(name, name, ybin,ymin,ymax);
      }
    }
    if (flag_detail){
      ybin=200;ymin=0.;ymax=2000.;
      for (UShort_t i=0;i<10;i++) {
        sprintf(name,"nai_cal_ch%d",i);
        nai_cal_ch[i]= new TH1F(name,name,ybin,ymin,ymax);
      }
      xbin=10;xmin=-0.5;xmax=9.5;
      hNaIEcal = new TH2F("hNaIEcal","hNaIEcal",xbin,xmin,xmax,ybin,ymin,ymax);
      hNaIEcal->GetXaxis()->SetTitle("NaI number");
      hNaIEcal->GetYaxis()->SetTitle("Energy (keV)");
      hNaIEcal->SetOption("COL");
    }
  }
  if (flag_ssd) {
  
    if (flag_raw){
      ybin=4096;ymin=0.;ymax=4095.;
      xbin=32;xmin=-0.5;xmax=31.5;
      for (UShort_t i=0;i<num_telescopes;i++) {
        sprintf(name,"hPsdStripE%d",i);
        hPsdStripE[i] = new TH2F(name,name,xbin,xmin,xmax,ybin,ymin,ymax);
        hPsdStripE[i]->SetOption("COL");
        sprintf(name,"hPsdStripT%d",i);
        hPsdStripT[i] = new TH2F(name,name,xbin,xmin,xmax,ybin,ymin,ymax);
        hPsdStripT[i]->SetOption("COL");
      }
      xbin=4096;xmin=0;xmax=4096;
      for (UShort_t i=0;i<num_strips;i++) {
        sprintf(name,"strip_ch%d",i);
        strip_ch[i]= new TH1F(name, name, xbin,xmin,xmax);
        sprintf(name,"strip_ch_gated%d",i);
        strip_ch_gated[i]= new TH1F(name, name,xbin,xmin,xmax);
      }
      for (UShort_t i=0;i<num_ssds;i++) {
        sprintf(name,"ssd_ch%d",i);
        ssd_ch[i]= new TH1F(name, name, xbin,xmin,xmax);
        sprintf(name,"ssd_ch_gated%d",i);
        ssd_ch_gated[i]= new TH1F(name, name,xbin,xmin,xmax);
      }
      xbin=15;xmin=-0.5;xmax=14.5;
      hSsdE = new TH2F("hSsdE","hSsdE",xbin,xmin,xmax,ybin,ymin,ymax);
      hSsdE->SetOption("COL");
      hSsdT = new TH2F("hSsdT","hSsdT",xbin,xmin,xmax,ybin,ymin,ymax);
      hSsdT->SetOption("COL");
    } // end if: flag_raw
  
    if (flag_detail){
      xbin=16;xmin=-0.5;xmax=15.5;
      ybin=1000;ymin=-1000;ymax=2000;
      for (UShort_t i=0;i<num_telescopes;i++) {
        sprintf(name,"hPsdStripTcal%d",i);
        hPsdStripTcal[i] = new TH2F(name,name,xbin,xmin,xmax,ybin,ymin,ymax);
        hPsdStripTcal[i]->SetOption("COL");
      }
      xbin=32;xmin=-0.5;xmax=31.5;
      ybin=700;ymin=0.;ymax=7.;
      for (UShort_t i=0;i<num_telescopes;i++) {
        sprintf(name,"hPsdStripEcal%d",i);
        hPsdStripEcal[i] = new TH2F(name,name,xbin,xmin,xmax,ybin,ymin,ymax);
        hPsdStripEcal[i]->SetOption("COL");
      }
      for (UShort_t i=0;i<num_strips;i++) {
        sprintf(name,"strip_cal_ch%d",i);
        strip_cal_ch[i]= new TH1F(name, name,ybin,ymin,ymax);
      }
      ybin=1000;ymin=0.;ymax=20.;
      for (UShort_t i=0;i<num_ssds;i++) {
        sprintf(name,"ssd_cal_ch%d",i);
        ssd_cal_ch[i]= new TH1F(name, name,ybin,ymin,ymax);
      }
      ybin=1000;ymin=0.;ymax=30.;
      for (UShort_t i=0;i<num_telescopes;i++) {
        sprintf(name,"hTelescopeELow%d",i);
        hTelescopeELow[i]= new TH1F(name, name,ybin,ymin,ymax);
      }
      ybin=280;ymin=0.;ymax=7.;
      for (UShort_t i=0;i<num_telescopes;i++) {
        sprintf(name,"hTelescopeEHigh%d",i);
        hTelescopeEHigh[i]= new TH1F(name, name,ybin,ymin,ymax);
        sprintf(name,"hTelescopeEHighcut%d",i);
        hTelescopeEHighcut[i]= new TH1F(name, name,ybin,ymin,ymax);
      }
      xbin=15;xmin=-0.5;xmax=14.5;
      hSsdEcal = new TH2F("hSsdEcal","hSsdEcal",xbin,xmin,xmax,ybin,ymin,ymax);
      hSsdEcal->SetOption("COL");
    } // end if: flag_detail

      hE1_E2= new TH2F("hE1_E2","hE1_E2", 50,0.,5., 50,0.,5.);
      hE1_E2->SetOption("COL");
    for (UShort_t i=0;i<num_telescopes;i++) {
      sprintf(name,"hDE_ELow%d",i);
      hDE_ELow[i]= new TH2F(name, name, 280,0.,7., 1200,0.,30.);
      hDE_ELow[i]->SetOption("COL");
      sprintf(name,"hDE_ELowcut%d",i);
      hDE_ELowcut[i]= new TH2F(name, name, 280,0.,7., 1200,0.,30.);
      hDE_ELowcut[i]->SetOption("COL");
      sprintf(name,"hDE_EHigh%d",i);
      hDE_EHigh[i]= new TH2F(name, name, 120,0.,6., 120,0.,6.);
      hDE_EHigh[i]->SetOption("COL");
      sprintf(name,"hPsdXY%d",i);
      hPsdXY[i]= new TH2F(name, name, 16,-0.5,15.5, 16,-0.5,15.5);
      hPsdXY[i]->SetOption("CONT");
      
      xbin=1200;xmin=0;xmax=30;ybin=500;ymin=-100;ymax=400;
      sprintf(name,"hTelescopeE_Tof%d",i);
      hTelescopeE_Tof[i]= new TH2F(name,name,xbin,xmin,xmax,ybin,ymin,ymax);
      hTelescopeE_Tof[i]->SetOption("COL");
      sprintf(name,"hTelescopeE_Tofcut%d",i);
      hTelescopeE_Tofcut[i]= new TH2F(name,name,xbin,xmin,xmax,ybin,ymin,ymax);
      hTelescopeE_Tofcut[i]->SetOption("COL");
      sprintf(name,"hProton%d",i);
      hProton[i] = new TH1F(name,name,600,0.,30.);
      sprintf(name,"hDeuteron%d",i);
      hDeuteron[i] = new TH1F(name,name,600,0.,30.);
    }
    xbin = 250;
    xmin = 0.0; xmax = 25.;
    hNstdc = new TH1F("hNstdc","hNstdc",xbin,xmin,xmax);

  } // end if: flag_detail & flag_ssd

if (flag_ppac && flag_ssd && flag_detail) {

    for (UShort_t i=0;i<2;i++) {
      xbin=130;xmin=0.;xmax=65.;
      ybin=240;ymin=20.;ymax=140.;
      sprintf(name,"hF2SsdRf%d",i);
      hF2SsdRf[i]= new TH2F(name, name, xbin,xmin,xmax,ybin,ymin,ymax);
      hF2SsdRf[i]->SetOption("COL");
    }
}

  if (flag_ppac){
  
    if(flag_raw){
      for (UShort_t i=0;i<2;i++){
        sprintf(name,"hRf_ds%d",i);
        hRf_ds[i] = new TH1F(name,name,240,0,120);
        sprintf(name,"hRf_ssd%d",i);
        hRf_ssd[i] = new TH1F(name,name,240,0,120);
      }
      xbin=600;xmin=-300.;xmax=300.;
      hPpac0TX1 = new TH1F("hPpac0TX1","PPACa X1",xbin,xmin,xmax);
      hPpac0TX1->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac0TX1->GetXaxis()->CenterTitle(true);
      hPpac0TX1->GetYaxis()->SetTitle("Counts");
      hPpac0TX1->GetYaxis()->CenterTitle(true);
      hPpac0TX2 = new TH1F("hPpac0TX2","PPACa X2",xbin,xmin,xmax);
      hPpac0TX2->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac0TX2->GetXaxis()->CenterTitle(true);
      hPpac0TX2->GetYaxis()->SetTitle("Counts");
      hPpac0TX2->GetYaxis()->CenterTitle(true);
      hPpac0TY1 = new TH1F("hPpac0TY1","PPACa Y1",xbin,xmin,xmax);
      hPpac0TY1->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac0TY1->GetXaxis()->CenterTitle(true);
      hPpac0TY1->GetYaxis()->SetTitle("Counts");
      hPpac0TY1->GetYaxis()->CenterTitle(true);
      hPpac0TY2 = new TH1F("hPpac0TY2","PPACa Y2",xbin,xmin,xmax);
      hPpac0TY2->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac0TY2->GetXaxis()->CenterTitle(true);
      hPpac0TY2->GetYaxis()->SetTitle("Counts");
      hPpac0TY2->GetYaxis()->CenterTitle(true);
      hPpac1TX1 = new TH1F("hPpac1TX1","PPACb X1",xbin,xmin,xmax);
      hPpac1TX1->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac1TX1->GetXaxis()->CenterTitle(true);
      hPpac1TX1->GetYaxis()->SetTitle("Counts");
      hPpac1TX1->GetYaxis()->CenterTitle(true);
      hPpac1TX2 = new TH1F("hPpac1TX2","PPACb X2",xbin,xmin,xmax);
      hPpac1TX2->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac1TX2->GetXaxis()->CenterTitle(true);
      hPpac1TX2->GetYaxis()->SetTitle("Counts");
      hPpac1TX2->GetYaxis()->CenterTitle(true);
      hPpac1TY1 = new TH1F("hPpac1TY1","PPACb Y1",xbin,xmin,xmax);
      hPpac1TY1->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac1TY1->GetXaxis()->CenterTitle(true);
      hPpac1TY1->GetYaxis()->SetTitle("Counts");
      hPpac1TY1->GetYaxis()->CenterTitle(true);
      hPpac1TY2 = new TH1F("hPpac1TY2","PPACb Y2",xbin,xmin,xmax);
      hPpac1TY2->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac1TY2->GetXaxis()->CenterTitle(true);
      hPpac1TY2->GetYaxis()->SetTitle("Counts");
      hPpac1TY2->GetYaxis()->CenterTitle(true);
      hPpac2TX1 = new TH1F("hPpac2TX1","F2 PPAC X1",xbin,xmin,xmax);
      hPpac2TX1->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac2TX1->GetXaxis()->CenterTitle(true);
      hPpac2TX1->GetYaxis()->SetTitle("Counts");
      hPpac2TX1->GetYaxis()->CenterTitle(true);
      hPpac2TX2 = new TH1F("hPpac2TX2","F2 PPAC X2",xbin,xmin,xmax);
      hPpac2TX2->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac2TX2->GetXaxis()->CenterTitle(true);
      hPpac2TX2->GetYaxis()->SetTitle("Counts");
      hPpac2TX2->GetYaxis()->CenterTitle(true);
      hPpac2TY1 = new TH1F("hPpac2TY1","F2 PPAC Y1",xbin,xmin,xmax);
      hPpac2TY1->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac2TY1->GetXaxis()->CenterTitle(true);
      hPpac2TY1->GetYaxis()->SetTitle("Counts");
      hPpac2TY1->GetYaxis()->CenterTitle(true);
      hPpac2TY2 = new TH1F("hPpac2TY2","F2 PPAC Y2",xbin,xmin,xmax);
      hPpac2TY2->GetXaxis()->SetTitle("Raw timing (ch)");
      hPpac2TY2->GetXaxis()->CenterTitle(true);
      hPpac2TY2->GetYaxis()->SetTitle("Counts");
      hPpac2TY2->GetYaxis()->CenterTitle(true);
    } // end if: flag_raw
    
    if (flag_detail){
      for (UShort_t i=0;i<2;i++){
        sprintf(name,"hRfcal%d",i);
        hRfcal[i] = new TH1F(name,name,160,-10.,70.);
        hRfcal[i]->GetXaxis()->SetTitle("RF (ns)");
	hRfcal[i]->GetXaxis()->CenterTitle(true);
        hRfcal[i]->GetYaxis()->SetTitle("counts");
	hRfcal[i]->GetYaxis()->CenterTitle(true);
      }
      hRF0Tof = new TH2F("hRF0Tof","RF0 vs. PPAC Tof",300,0.,60.,200,0.,20.);
      hRF0Tof->SetOption("COL");
      hRF0Tof->GetXaxis()->SetTitle("RF (ns)");
      hRF0Tof->GetXaxis()->CenterTitle(true);
      hRF0Tof->GetYaxis()->SetTitle("Time of Flight (ns)");
      hRF0Tof->GetYaxis()->CenterTitle(true);
      
      hRF1Tof = new TH2F("hRF1Tof","RF1 vs. PPAC Tof",300,0.,60.,200,0.,20.);
      hRF1Tof->SetOption("COL");
      hRF1Tof->GetXaxis()->SetTitle("RF (ns)");
      hRF1Tof->GetXaxis()->CenterTitle(true);
      hRF1Tof->GetYaxis()->SetTitle("Time of Flight (ns)");
      hRF1Tof->GetYaxis()->CenterTitle(true);
      
      hPpac0XY = new TH2F("hPpac0XY","PPACa X vs. Y",160,-40.,40.,160,-40.,40.);
      hPpac0XY->SetOption("COL");
      hPpac0XY->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XY->GetXaxis()->CenterTitle(true);
      hPpac0XY->GetYaxis()->SetTitle("Y Position (mm)");
      hPpac0XY->GetYaxis()->CenterTitle(true);
      
      hPpac0XYcut = new TH2F("hPpac0XYcut","PPACa X vs. Y",160,-40.,40.,160,-40.,40.);
      hPpac0XYcut->SetOption("COL");
      hPpac0XYcut->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XYcut->GetXaxis()->CenterTitle(true);
      hPpac0XYcut->GetYaxis()->SetTitle("Y Position (mm)");
      hPpac0XYcut->GetYaxis()->CenterTitle(true);
      
      hPpac1XY = new TH2F("hPpac1XY","PPACb X vs. Y",160,-40.,40.,160,-40.,40.);
      hPpac1XY->SetOption("COL");   
      hPpac1XY->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XY->GetXaxis()->CenterTitle(true);
      hPpac1XY->GetYaxis()->SetTitle("Y Position (mm)");
      hPpac1XY->GetYaxis()->CenterTitle(true);
      
      hPpac1XYcut = new TH2F("hPpac1XYcut","PPACb X vs. Y",160,-40.,40.,160,-40.,40.);
      hPpac1XYcut->SetOption("COL");   
      hPpac1XYcut->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XYcut->GetXaxis()->CenterTitle(true);
      hPpac1XYcut->GetYaxis()->SetTitle("Y Position (mm)");
      hPpac1XYcut->GetYaxis()->CenterTitle(true);
      
      hPpac2XY = new TH2F("hPpac2XY","F2 PPAC X vs. Y",160,-40.,40.,160,-40.,40.);
      hPpac2XY->SetOption("COL");
      hPpac2XY->GetXaxis()->SetTitle("X Position (mm)");
      hPpac2XY->GetXaxis()->CenterTitle(true);
      hPpac2XY->GetYaxis()->SetTitle("Y Position (mm)");
      hPpac2XY->GetYaxis()->CenterTitle(true);
      
      hPpac2XYcut = new TH2F("hPpac2XYcut","F2 PPAC X vs. Y",160,-40.,40.,160,-40.,40.);
      hPpac2XYcut->SetOption("COL");
      hPpac2XYcut->GetXaxis()->SetTitle("X Position (mm)");
      hPpac2XYcut->GetXaxis()->CenterTitle(true);
      hPpac2XYcut->GetYaxis()->SetTitle("Y Position (mm)");
      hPpac2XYcut->GetYaxis()->CenterTitle(true);
      
      hTargetXY = new TH2F("hTargetXY","Target X vs. Y",8320,-40.,40.,320,-40.,40.);
      hTargetXY->SetOption("COL");   
      hTargetXY->GetXaxis()->SetTitle("X Position (mm)");
      hTargetXY->GetXaxis()->CenterTitle(true);
      hTargetXY->GetYaxis()->SetTitle("Y Position (mm)");
      hTargetXY->GetYaxis()->CenterTitle(true);
      
      hTargetXYcut = new TH2F("hTargetXYcut","Target X vs. Y window cut",320,-40.,40.,320,-40.,40.);
      hTargetXYcut->SetOption("COL");   
      hTargetXYcut->GetXaxis()->SetTitle("X Position (mm)");
      hTargetXYcut->GetXaxis()->CenterTitle(true);
      hTargetXYcut->GetYaxis()->SetTitle("Y Position (mm)");
      hTargetXYcut->GetYaxis()->CenterTitle(true);
      
      hTargetXY_26al = new TH2F("hTargetXY_26al","^{26}Al Target X vs. Y",320,-40.,40.,320,-40.,40.);
      hTargetXY_26al->SetOption("COL");   
      hTargetXY_26al->GetXaxis()->SetTitle("X Position (mm)");
      hTargetXY_26al->GetXaxis()->CenterTitle(true);
      hTargetXY_26al->GetYaxis()->SetTitle("Y Position (mm)");
      hTargetXY_26al->GetYaxis()->CenterTitle(true);
      
      hTargetXYcut_26al = new TH2F("hTargetXYcut_26al","^{26}Al Target X vs. Y window cut",320,-40.,40.,320,-40.,40.);
      hTargetXYcut_26al->SetOption("COL");   
      hTargetXYcut_26al->GetXaxis()->SetTitle("X Position (mm)");
      hTargetXYcut_26al->GetXaxis()->CenterTitle(true);
      hTargetXYcut_26al->GetYaxis()->SetTitle("Y Position (mm)");
      hTargetXYcut_26al->GetYaxis()->CenterTitle(true);
     
      xbin=160;xmin=-40.;xmax=40.;
      ybin=210;ymin=-70.;ymax=140.;
      //ybin=280;ymin=0.;ymax=140.;
      //ybin=130;ymin=0.;ymax=65.;

      hPpac0XRF0 = new TH2F("hPpac0XRF0","PPACa X vs. RF0",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF0->SetOption("COL");
      hPpac0XRF0->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF0->GetXaxis()->CenterTitle(true);
      hPpac0XRF0->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF0->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF1 = new TH2F("hPpac0XRF1","PPACa X vs. RF1",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF1->SetOption("COL");
      hPpac0XRF1->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF1->GetXaxis()->CenterTitle(true);
      hPpac0XRF1->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF1->GetYaxis()->CenterTitle(true);
      
      hPpac1XRF0 = new TH2F("hPpac1XRF0","PPACb X vs. RF0",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF0->SetOption("COL");
      hPpac1XRF0->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF0->GetXaxis()->CenterTitle(true);
      hPpac1XRF0->GetYaxis()->SetTitle("RF (ns)");
      hPpac1XRF0->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF0_26al = new TH2F("hPpac0XRF0_26al","PPACa X vs. RF0 26Al",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF0_26al->SetOption("COL");
      hPpac0XRF0_26al->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF0_26al->GetXaxis()->CenterTitle(true);
      hPpac0XRF0_26al->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF0_26al->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF0_26al_ds = new TH2F("hPpac0XRF0_26al_ds","PPACa X vs. RF0 26Al DS",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF0_26al_ds->SetOption("COL");
      hPpac0XRF0_26al_ds->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF0_26al_ds->GetXaxis()->CenterTitle(true);
      hPpac0XRF0_26al_ds->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF0_26al_ds->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF1_26al = new TH2F("hPpac0XRF1_26al","PPACa X vs. RF1 26Al",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF1_26al->SetOption("COL");
      hPpac0XRF1_26al->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF1_26al->GetXaxis()->CenterTitle(true);
      hPpac0XRF1_26al->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF1_26al->GetYaxis()->CenterTitle(true);
      
      hPpac1XRF0_26al = new TH2F("hPpac1XRF0_26al","PPACb X vs. RF0 26Al",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF0_26al->SetOption("COL");
      hPpac1XRF0_26al->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF0_26al->GetXaxis()->CenterTitle(true);
      hPpac1XRF0_26al->GetYaxis()->SetTitle("RF (ns)");
      hPpac1XRF0_26al->GetYaxis()->CenterTitle(true);
      
      hPpac1XRF1_26al = new TH2F("hPpac1XRF1_26al","PPACb X vs. RF1 26Al",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF1_26al->SetOption("COL");
      hPpac1XRF1_26al->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF1_26al->GetXaxis()->CenterTitle(true);
      hPpac1XRF1_26al->GetYaxis()->SetTitle("RF (ns)");
      hPpac1XRF1_26al->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF1cut = new TH2F("hPpac0XRF1cut","PPACa X vs. RF1",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF1cut->SetOption("COL");
      hPpac0XRF1cut->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF1cut->GetXaxis()->CenterTitle(true);
      hPpac0XRF1cut->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF1cut->GetYaxis()->CenterTitle(true);
      
      hPpac1XRF1cut = new TH2F("hPpac1XRF1cut","PPACb X vs. RF1",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF1cut->SetOption("COL");
      hPpac1XRF1cut->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF1cut->GetXaxis()->CenterTitle(true);
      hPpac1XRF1cut->GetYaxis()->SetTitle("RF (ns)");
      hPpac1XRF1cut->GetYaxis()->CenterTitle(true);
      
      hPpac1XRF1 = new TH2F("hPpac1XRF1","PPACb X vs. RF1",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF1->SetOption("COL");
      hPpac1XRF1->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF1->GetXaxis()->CenterTitle(true);
      hPpac1XRF1->GetYaxis()->SetTitle("RF (ns)");
      hPpac1XRF1->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF0ds = new TH2F("hPpac0XRF0ds","PPACa X vs. RF0 downscale",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF0ds->SetOption("COL");
      hPpac0XRF0ds->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF0ds->GetXaxis()->CenterTitle(true);
      hPpac0XRF0ds->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF0ds->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF1ds = new TH2F("hPpac0XRF1ds","PPACa X vs. RF1 downscale",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF1ds->SetOption("COL");
      hPpac0XRF1ds->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF1ds->GetXaxis()->CenterTitle(true);
      hPpac0XRF1ds->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF1ds->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF0ssd = new TH2F("hPpac0XRF0ssd","PPACa X vs. RF0 SSD-OR",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF0ssd->SetOption("COL");
      hPpac0XRF0ssd->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF0ssd->GetXaxis()->CenterTitle(true);
      hPpac0XRF0ssd->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF0ssd->GetYaxis()->CenterTitle(true);
      
      hPpac0XRF1ssd = new TH2F("hPpac0XRF1ssd","PPACa X vs. RF1 SSD-OR",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac0XRF1ssd->SetOption("COL");
      hPpac0XRF1ssd->GetXaxis()->SetTitle("X Position (mm)");
      hPpac0XRF1ssd->GetXaxis()->CenterTitle(true);
      hPpac0XRF1ssd->GetYaxis()->SetTitle("RF (ns)");
      hPpac0XRF1ssd->GetYaxis()->CenterTitle(true);
      
      hPpac1XRF0ds = new TH2F("hPpac1XRF0ds","PPACb X vs. RF0 downscale",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF0ds->SetOption("COL");
      hPpac1XRF0ds->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF0ds->GetXaxis()->CenterTitle(true);
      hPpac1XRF0ds->GetYaxis()->SetTitle("RF (ns)");
      hPpac1XRF0ds->GetYaxis()->CenterTitle(true);
      
      hPpac1XRF1ds = new TH2F("hPpac1XRF1ds","PPACb X vs. RF1 downscale",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF1ds->SetOption("COL");
      hPpac1XRF1ds->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF1ds->GetXaxis()->CenterTitle(true);
      hPpac1XRF1ds->GetYaxis()->SetTitle("RF (ns");
      hPpac1XRF1ds->GetYaxis()->CenterTitle(true);
      
      hPpac1XRF0ssd = new TH2F("hPpac1XRF0ssd","PPACb X vs. RF0 SSD-OR",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF0ssd->SetOption("COL");
      hPpac1XRF0ssd->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF0ssd->GetXaxis()->CenterTitle(true);
      hPpac1XRF0ssd->GetYaxis()->SetTitle("RF (ns)");
      hPpac1XRF0ssd->GetYaxis()->CenterTitle(true);

      hPpac1XRF1ssd = new TH2F("hPpac1XRF1ssd","PPACb X vs. RF1 SSD-OR",xbin,xmin,xmax,ybin,ymin,ymax);
      hPpac1XRF1ssd->SetOption("COL");
      hPpac1XRF1ssd->GetXaxis()->SetTitle("X Position (mm)");
      hPpac1XRF1ssd->GetXaxis()->CenterTitle(true);
      hPpac1XRF1ssd->GetYaxis()->SetTitle("RF (ns)");
      hPpac1XRF1ssd->GetYaxis()->CenterTitle(true);

      
    } // end if: flag_detail
  
  } // end if: flag_ppac
  
} // close HistInit

/**
@brief Write all the histograms.
@details This function should be called after all histograms have been Fill'd in Analyzer.\n
It will write all histograms that were initalized based on the data options.\n
*/
void HistWrite() {
  // If you have a lot of blind Seg Faults, this area is often guilty
  // if there is a mismatch between what's defined for various flags
  //return;
  // WRITE THE HISTOGRAMS


  if (flag_nai) {
    if (flag_raw){
      hNaIE->Write();
      for (UShort_t i=0;i<10;i++) {
        nai_ch[i]->Write();
      }
    }
    if (flag_detail){
      for (UShort_t i=0;i<10;i++) {
        nai_cal_ch[i]->Write();
      }
      hNaIEcal->Write();
    }
  }

  if (flag_ssd) { // process SSD?
    if (flag_raw){ // write the SSD strip raw histograms
      for (int i=0;i<num_telescopes;i++){
        hPsdStripE[i]->Write(); 
        hPsdStripT[i]->Write(); 
      }
      for (UShort_t i=0;i<num_strips;i++) {
         strip_ch[i]->Write();
         strip_ch_gated[i]->Write();
       }
      for (UShort_t i=0;i<num_ssds;i++) {
         ssd_ch[i]->Write();
         ssd_ch_gated[i]->Write();
       }
      hSsdE->Write(); 
      hSsdT->Write(); 
    } // end if: flag_raw
    
    if (flag_detail){ // write the SSD strip detailed histograms
      hSsdEcal->Write(); 
      for (int i=0;i<num_telescopes;i++){
        hPsdStripTcal[i]->Write(); 
        hPsdStripEcal[i]->Write(); 
      }
      for (UShort_t i=0;i<num_strips;i++) {
         strip_cal_ch[i]->Write();
      }
      for (UShort_t i=0;i<num_ssds;i++) {
         ssd_cal_ch[i]->Write();
      }
      for (UShort_t i=0;i<num_telescopes;i++) {
        hDE_ELow[i]->Write();
        hDE_ELowcut[i]->Write();
        hDE_EHigh[i]->Write();
        hPsdXY[i]->Write();
	hTelescopeELow[i]->Write();
	hTelescopeEHigh[i]->Write();
	hTelescopeEHighcut[i]->Write();
        hTelescopeE_Tof[i]->Write();
        hTelescopeE_Tofcut[i]->Write();
        hProton[i]->Write();
        hDeuteron[i]->Write();
      }
      hE1_E2->Write();
    hNstdc->Write();
    } // end if : flag_detail
  } //end if: flag_ssd

if (flag_ppac && flag_ssd && flag_detail) {
  hF2SsdRf[0]->Write();
  hF2SsdRf[1]->Write();
}
  if (flag_ppac){ // process PPAC?
    if (flag_raw) { // write the PPAC raw histograms
       for (UShort_t i=0;i<2;i++) hRf_ssd[i]->Write();
       for (UShort_t i=0;i<2;i++) hRf_ds[i]->Write();
       hPpac0TX1->Write();
       hPpac0TX2->Write();
       hPpac0TY1->Write();
       hPpac0TY2->Write();
       hPpac1TX1->Write();
       hPpac1TX2->Write();
       hPpac1TY1->Write();
       hPpac1TY2->Write();
       hPpac2TX1->Write();
       hPpac2TX2->Write();
       hPpac2TY1->Write();
       hPpac2TY2->Write();
    } // end if: flag_raw

    if (flag_detail) { // write the PPAC detailed histograms
       for (UShort_t i=0;i<2;i++) hRfcal[i]->Write();
       hRF0Tof->Write();
       hRF1Tof->Write();
       hPpac0XY->Write();
       hPpac0XYcut->Write();
       hPpac1XY->Write();  
       hPpac1XYcut->Write();  
       hPpac2XY->Write();  
       hPpac2XYcut->Write();  
       hTargetXY->Write();  
       hTargetXYcut->Write();  
       hTargetXY_26al->Write();  
       hTargetXYcut_26al->Write();  
       hPpac0XRF0->Write(); 
       hPpac1XRF0->Write(); 
       hPpac0XRF1->Write(); 
       hPpac0XRF1cut->Write(); 
       hPpac1XRF1->Write();
       hPpac1XRF1cut->Write(); 
       hPpac0XRF0_26al->Write(); 
       hPpac0XRF0_26al_ds->Write(); 
       hPpac0XRF1_26al->Write(); 
       hPpac1XRF0_26al->Write(); 
       hPpac1XRF1_26al->Write(); 
       hPpac0XRF0ds->Write(); 
       hPpac0XRF1ds->Write(); 
       hPpac0XRF0ssd->Write(); 
       hPpac0XRF1ssd->Write(); 
       hPpac1XRF0ds->Write(); 
       hPpac1XRF1ds->Write(); 
       hPpac1XRF0ssd->Write(); 
       hPpac1XRF1ssd->Write(); 
    } // end if: flag_detail
  } // end if: flag_ppac

} // close HistWrite


/**
@brief delete all the histograms
@details This function should be called after all histograms have been Write'n.\n
In principle it is not totally necessary to do this kind of garbage collection.\n
However, for memory checking with valgrind, this is helpful.
*/
void HistClean(){ // function to free histogram memory
  
  delete hNaIE;
  delete hNaIEcal;
  for (int i=0;i<10;i++){
    delete nai_ch[i] ;
    delete nai_cal_ch[i] ;
  }
  for (int i=0;i<num_telescopes;i++){
    delete hPsdStripE[i] ;
    delete hPsdStripT[i] ;
    delete hPsdStripEcal[i] ;
    delete hPsdStripTcal[i] ;
  }
  delete hF2SsdRf[0];
  delete hF2SsdRf[1];
  delete hNstdc;
  delete hSsdE ;
  delete hSsdT ;
  delete hSsdEcal ;
  delete hRF0Tof;
  delete hRF1Tof;
  delete hPpac0TX1;
  delete hPpac0TX2;
  delete hPpac0TY1;
  delete hPpac0TY2;
  delete hPpac1TX1;
  delete hPpac1TX2;
  delete hPpac1TY1;
  delete hPpac1TY2;
  delete hPpac2TX1;
  delete hPpac2TX2;
  delete hPpac2TY1;
  delete hPpac2TY2;
  delete hPpac0XY ;
  delete hPpac0XYcut ;
  delete hPpac1XY ;
  delete hPpac1XYcut ;
  delete hPpac2XY ;
  delete hPpac2XYcut ;
  delete hTargetXY ;
  delete hTargetXYcut ;
  delete hTargetXY_26al ;
  delete hTargetXYcut_26al ;
  delete hPpac0XRF0 ;
  delete hPpac0XRF0_26al ;
  delete hPpac0XRF0_26al_ds ;
  delete hPpac0XRF1_26al ;
  delete hPpac0XRF0ds ;
  delete hPpac0XRF1ds ;
  delete hPpac0XRF0ssd ;
  delete hPpac0XRF1ssd ;
  delete hPpac1XRF0 ;
  delete hPpac1XRF0_26al ;
  delete hPpac1XRF1_26al ;
  delete hPpac1XRF0ds ;
  delete hPpac1XRF1ds ;
  delete hPpac1XRF0ssd ;
  delete hPpac1XRF1ssd ;
  delete hPpac0XRF1 ;
  delete hPpac0XRF1cut ;
  delete hPpac1XRF1cut ;
  delete hPpac1XRF1 ;
  for (int i=0;i<2;i++) delete hRf_ds[i]; 
  for (int i=0;i<2;i++) delete hRf_ssd[i]; 
  for (int i=0;i<2;i++) delete hRfcal[i]; 
  for (int i=0;i<num_strips;i++){
    delete strip_ch[i];
    delete strip_ch_gated[i];
    delete strip_cal_ch[i];
  }
  for (int i=0;i<num_ssds;i++){
    delete ssd_ch[i];
    delete ssd_ch_gated[i];
    delete ssd_cal_ch[i];
  }
  delete hE1_E2;
  for (int i=0;i<num_telescopes;i++){
    delete hDE_ELow[i];
    delete hDE_ELowcut[i];
    delete hDE_EHigh[i];
    delete hPsdXY[i];
    delete hTelescopeELow[i];
    delete hTelescopeEHigh[i];
    delete hTelescopeEHighcut[i];
    delete hTelescopeE_Tof[i];
    delete hTelescopeE_Tofcut[i];
    delete hProton[i];
    delete hDeuteron[i];
  }
} // close HistClean    
