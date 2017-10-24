/*
 Copyright: 
 2010 daid KAHL, OTA shinsuke, HASHIMOTO takashi, YAMAGUCHI hidetoshi 
 2011, 2012, 2013, 2015, 2016 daid KAHL
 2015 KIM Dahee

 This file is modified from crabat

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
@file Analyzer.cxx
@author daid, Dahee
@version 1.0
@brief Analyzer for the run data.
@details Takes the Chain and loops over all entries.\n
Fill histograms, do physics.
@date 07 Nov 2011 18:57:28 
*/

//////////////////////////////////////
//    analyze the data here!
/////////////////////////////////////
// If run from ROOT, exit with message
#if defined (__CINT__)
{
  cout << "This is C++ code, not a ROOT macro!" << endl;
  cout << "To continue, run:" << endl;
  cout << "make" << endl;
  cout << "./run" << endl;
  gApplication->Terminate();
}
#endif

#define Analyzer_cxx

#include "Analyzer.h" // all other headers are here
#include "Analyzer_config.cxx"
#include "Calibration.cxx"

#include "common.h"

// hopefully we don't need it!
//#include "libs.hh"

/**
 * @brief Loops over the TChain and entries
 * @details Oh my god there are so many details here.
 */
void Analyzer::Loop(Int_t run,
     Bool_t flag_raw, Bool_t flag_detail,
     Bool_t flag_ssd, Bool_t flag_ppac, Bool_t flag_nai)
{
  if (fChain == 0) return;

  TFile *fCut1 =new TFile("cuts/proton_tel2.root");
  TCutG* cut_de_e_proton_tel2 = (TCutG*)gROOT->FindObject("proton_tel2");
  //// TODO: clone cut_e_tof_tel2 into cut_t_tof_tel1 in this root file...there was a mistake
  //TFile *fCut2 =new TFile("cuts/cut_e_tof_tel1.root");
  //TCutG* cut_e_tof_tel1 = (TCutG*)gROOT->FindObject("cut_e_tof_tel2");
  TFile *fCut3 =new TFile("cuts/cut_e_tof_tel2.root");
  TCutG* cut_e_tof_tel2 = (TCutG*)gROOT->FindObject("cut_e_tof_tel2");
  TFile *fCut4 =new TFile("cuts/deuteron_tel2.root");
  TCutG* cut_de_e_deuteron_tel2 = (TCutG*)gROOT->FindObject("cut_de_e_deuteron_tel2");
  TFile *fCut5 =new TFile("cuts/cut_e_tof_d_tel2.root");
  TCutG* cut_e_tof_d_tel2 = (TCutG*)gROOT->FindObject("cut_e_tof_d_tel2");

  Bool_t gate_26al=false;
  Bool_t proton_beam=false;
  Bool_t ribeam_ssd=false;
  Bool_t goodRF_26al=false, goodPpacX_26al=false;
  Bool_t hit_target=false;

  Long64_t nentries = fChain->GetEntries();
  //Long64_t nentries = fChain->GetEntriesFast(); bugged somehow...always makes a number that is too large
  Calibration *psd_calib_t;  //=new Calibration("calib/psd_calib_t.dat",96);;
  if (run <88)
    psd_calib_t=new Calibration("calib/psd_calib_t_r1-87.dat",96);//
  else
    psd_calib_t=new Calibration("calib/psd_calib_t_r88plus.dat",96);//
  Calibration *psd_calib=new Calibration("calib/psd_calib.dat",96);//
  if (flag_detail && flag_ssd){
    psd_calib_t->ScanFile();
    psd_calib_t->ShowEntries();
    psd_calib->ScanFile();
    psd_calib->ShowEntries();
  }
  Calibration *ssd_calib=new Calibration("calib/ssd_calib.dat",15);//
  // load the appropriate calibration
  if (flag_detail && flag_ssd){
    ssd_calib->ScanFile();
    ssd_calib->ShowEntries();
  }
  Calibration *nai_calib=new Calibration("calib/nai_calib.dat",10);//
  if (flag_detail && flag_nai){
    nai_calib->ScanFile();
    nai_calib->ShowEntries();
  }
  static const UShort_t PpacSepZ = SetPpacSepZ();
  // target projection function
  // PPACa set as 0 position
  fTargetX = new TF1("fTargetX","[0]+((x/[2])*([1]-[0]))",0,1000); 
  // PPACb set as 0 -- idential results if we use the target position which subtracts [2] for calling
  //    or we could make perfectly identical results if we summed that into the function itself
  //fTargetX = new TF1("fTargetX","[0]+((x+[2])*(([1]-[0])/[2]))",0,1000); 
  fTargetX->SetParNames("PPACaX","PPACbX","PPAC Separation");
  fTargetY = new TF1("fTargetY","[0]+((x/[2])*([1]-[0]))",0,1000);
  fTargetY->SetParNames("PPACaY","PPACbY","PPAC Separation");
  // TODO: Confirm values for March 2017 from operators
  // from macros/rf_shift.C 
  /*
	=== RF0 ===
	Set     Peak 0          Peak 1          Delta           Shift
	0       42.9935         102.322         59.3282         -24.9146
	1       18.0788         79.361          61.2821         0
	2       7.90395         68.997          61.093          10.1749
	3       17.7335         79.0016         61.2682         0.345369
	4       24.1699         85.4664         61.2966         -6.09103
	5       14.3485         75.5473         61.1989         3.73036
	
	Average:                                61.2277
	
	=== RF1 ===
	Set     Peak 0          Peak 1          Delta           Shift
	0       31.9655         93.0474         61.0819         -24.1983
	1       7.76724         68.2702         60.503          0
	2       -0.53215        57.9569         58.489          8.29939
	3       7.48953         68.0005         60.511          0.277708
	4       13.5623         74.4096         60.8473         -5.79508
	5       4.4102          64.5308         60.1206         3.35704
	
	Average:                                60.6128
	
*/
  const double rf_period[2]={61.227,60.6128}; 
  // TODO: FIX ME
  const double WindowZ=765.0; // distance of window position from center of PPACa

  //===============================================================
	Long64_t nbytes = 0, nb = 0;	
  //===============================================================

  for(Long64_t jentry=0; jentry<nentries;jentry++) {
    //cout << "***********************************" << endl;
    nb = fChain->GetEntry(jentry); 
    
    if(jentry%10000 == 0) // Gives feedback on the analysis status
    {
    	cout << jentry << " / " << nentries << "\r" << flush;
    }

    // when you only want to analyze a portion 
    // of the data for a quick check, try something like:
    //if (jentry > 10000) break;

    if (flag_ppac){
      if(flag_raw){
        /*
	for (int nPpac=0;nPpac<3;nPpac++){
	  for (int ch=0;ch<4;ch++){
            if (fPpac[nPpac][ch] > 0.) hPpac[nPpac][ch]->Fill(fPpac[nPpac][ch]);
	  }
	}
        */
	tof[0]=-1000;tof[1]=-1000;tof[2]=-1000;
	if (fPpac[0][0]>0.) hPpac0TX1->Fill(fPpac[0][0]);
        if (fPpac[0][1]>0.) hPpac0TX2->Fill(fPpac[0][1]);
        if (fPpac[0][2]>0.) hPpac0TY1->Fill(fPpac[0][2]);
        if (fPpac[0][3]>0.) hPpac0TY2->Fill(fPpac[0][3]);
        if (fPpac[1][0]>0.) hPpac1TX1->Fill(fPpac[1][0]);
        if (fPpac[1][1]>0.) hPpac1TX2->Fill(fPpac[1][1]);
        if (fPpac[1][2]>0.) hPpac1TY1->Fill(fPpac[1][2]);
        if (fPpac[1][3]>0.) hPpac1TY2->Fill(fPpac[1][3]);
        if (fPpac[2][0]>0.) hPpac2TX1->Fill(fPpac[2][0]);
        if (fPpac[2][1]>0.) hPpac2TX2->Fill(fPpac[2][1]);
        if (fPpac[2][2]>0.) hPpac2TY1->Fill(fPpac[2][2]);
        if (fPpac[2][3]>0.) hPpac2TY2->Fill(fPpac[2][3]);

        for(int nRf=0;nRf<2;nRf++){
          // overflow data go to exactly 0
	  // there may be very few legitimate events with nearly 0 relative flight time
	  // but this is an okay compromise
	  if (fRf[nRf]>0.2){
            if (fCr[0]) hRf_ssd[nRf]->Fill(fRf[nRf]);
            if (fCr[1]) hRf_ds[nRf]->Fill(fRf[nRf]);
          }
        }
      } 
      if(flag_detail){
        
        // RF calibration 17 May 2017 22:15:41  
        // TODO: goodRF type flag?
	for(int nRf=0;nRf<2;nRf++){
	  Rfcal[nRf]=-1000.;
	  if (fRf[nRf]>0.5){
              Rfcal[nRf]=fRf[nRf];
    	    Rfcal[nRf]=Rfcal[nRf]+GetRfShift(nRf,run);
    	    //Rfcal[nRf]+=RfShift(run,nRf);
            // rf wrap-around calibration
	    
	    //float temp = Rfcal[nRf];
	    
	    if (Rfcal[nRf]<0.){
              Rfcal[nRf]=(Rfcal[nRf]+rf_period[nRf]);
	    }
	    // TODO: Q/A check on this large RF overflow?  Should we really wrap it?
	    //if (Rfcal[nRf]>rf_period[nRf]){
	    while (Rfcal[nRf]>rf_period[nRf]){
	    // run 103 has RF1 ~130 sometimes ... maybe corrupt but... 
              Rfcal[nRf]=(Rfcal[nRf]-rf_period[nRf]);
	    }
	    /*
	    if (Rfcal[nRf]>rf_period[nRf]){
		cout << "\t\t" << temp << "\t" << Rfcal[nRf] << endl;
	    }
	    */
	    hRfcal[nRf]->Fill(Rfcal[nRf]);
          }
        }
        
        // Check if each PPAC is hit with valid event
        // not sure the origin of the stuff higher than 220 but it looks clearly like junk in the 
        //   raw timing spectrum and shows up roughly near the zero point
        for(UShort_t i=0;i<3;i++){
          PpacIsHit[i] = false;
          if (fPpac[i][0]>0. && fPpac[i][1]>0. && fPpac[i][2]>0. && fPpac[i][3]>0.
           && fPpac[i][0]<220. && fPpac[i][1]<220. && fPpac[i][2]<220. && fPpac[i][3]<220. )
            PpacIsHit[i]=true;
        }
	if (PpacXYCalib(flag_detail,flag_ppac,run)) cout << "PPAC XY Calibration failed!" << endl;


        
        if (PpacIsHit[0]) hPpac0XY->Fill(PpacX[0],PpacY[0]);
        if (PpacIsHit[1]) hPpac1XY->Fill(PpacX[1],PpacY[1]);
        if (PpacIsHit[2]) hPpac2XY->Fill(PpacX[2],PpacY[2]);
        /*
	if (PpacX[1]<0.60 && PpacX[1]>-0.01 && PpacY[1]<-0.90 && PpacY[1]>-1.6) {
        	//cout << PpacX[0] << "\t" << PpacY[0] << endl;
          cout << fPpac[1][0] << "\t" << fPpac[1][1] << "\t" << fPpac[1][2] << "\t" << fPpac[1][3] << endl;
        
        
        }
	*/
        // Turn off all the gates for a new event
        gate_26al=false;
        goodRF_26al=false;
        goodPpacX_26al=false;
        // gates by run 83 17 May 2017 19:17:42 
        if (PpacIsHit[0] || PpacIsHit[1]){
        //if (PpacIsHit[0] && PpacIsHit[1]){
          //if (  ((Rfcal[0]>=10.) && (Rfcal[0]<=24.)) )
//          if (  ((Rfcal[0]>=10.) && (Rfcal[0]<=24.)) && ((Rfcal[1]>=60.2)&&(Rfcal[1]<=72.)) )
//          if (  ((Rfcal[0]>=10.) && (Rfcal[0]<=24.)) || ((Rfcal[1]>=60.2)&&(Rfcal[1]<=72.)) )
          if (  ((Rfcal[0]>=12.) && (Rfcal[0]<=23.)) || ((Rfcal[1]>=2.0)&&(Rfcal[1]<=13.)) )
          //if (  ((Rfcal[0]>=10.) && (Rfcal[0]<=24.)) && ((Rfcal[1]>=0.2)&&(Rfcal[1]<=12.)) ) // nothing comes out
            {goodRF_26al=true;} // 26Al gate
          // TODO: Fix this...maybe not useful if we gate instead on the target frame later
	  if ((PpacX[0]>=-18. && PpacX[0]<=18.) && (PpacX[1]>=-24. && PpacX[1]<=30.)) {goodPpacX_26al=true;} // 26Al gate
          if (goodRF_26al) {gate_26al=true;}
          //if (goodRF_26al && goodPpacX_26al) {gate_26al=true;}
        }
    	if (Rfcal[0] > -100){
          // TODO: goodRF
    	  if (PpacIsHit[0] ){
                  hPpac0XRF0->Fill(PpacX[0],Rfcal[0]);
                  if (fCr[0]) hPpac0XRF0ssd->Fill(PpacX[0],Rfcal[0]);
                  if (fCr[1]) hPpac0XRF0ds->Fill(PpacX[0],Rfcal[0]);
          }
    	  if (PpacIsHit[1] ){
                hPpac1XRF0->Fill(PpacX[1],Rfcal[0]);
                if (fCr[0]) hPpac1XRF0ssd->Fill(PpacX[1],Rfcal[0]);
                if (fCr[1]) hPpac1XRF0ds->Fill(PpacX[1],Rfcal[0]);
          }
    	  if (PpacIsHit[2] ){
          }
        }
          // TODO: goodRF
        if (Rfcal[1] > -100){
    	  if (PpacIsHit[0] ){
            hPpac0XRF1->Fill(PpacX[0],Rfcal[1]);
            if (fCr[0]) hPpac0XRF1ssd->Fill(PpacX[0],Rfcal[1]);
            if (fCr[1]) hPpac0XRF1ds->Fill(PpacX[0],Rfcal[1]);
          }
          if (PpacIsHit[1] ){
            hPpac1XRF1->Fill(PpacX[1],Rfcal[1]);
            if (fCr[0]) hPpac1XRF1ssd->Fill(PpacX[1],Rfcal[1]);
            if (fCr[1]) hPpac1XRF1ds->Fill(PpacX[1],Rfcal[1]);
          }
        }
    	
    	if (gate_26al){
          if (PpacIsHit[0] && Rfcal[0]>-100) {
	    hPpac0XRF0_26al->Fill(PpacX[0],Rfcal[0]);
            if (fCr[1]) hPpac0XRF0_26al_ds->Fill(PpacX[0],Rfcal[0]);
	  }
          if (PpacIsHit[1] && Rfcal[0]>-100) {
	    hPpac1XRF0_26al->Fill(PpacX[1],Rfcal[0]);
	  }
          // TODO: goodRF
	  if (PpacIsHit[0] && Rfcal[1]>-100) {
	    hPpac0XRF1_26al->Fill(PpacX[0],Rfcal[1]);
	  }
          if (PpacIsHit[1] && Rfcal[1]>-100) hPpac1XRF1_26al->Fill(PpacX[1],Rfcal[1]);
        }
    
    /*
     * Consumes a lot of resources  ... 
    
        fTargetX->SetParameters(PpacX[0],PpacX[1],PpacSepZ);
        fTargetY->SetParameters(PpacY[0],PpacY[1],PpacSepZ);
        hit_target=false;
        if(WindowCut(fTargetX->Eval(WindowZ),fTargetY->Eval(WindowZ))) hit_target=true; 
      //reset
        TargetX=-100.; 
        TargetY=-100.;
        TargetX=fTargetX->Eval(WindowZ);
        TargetY=fTargetY->Eval(WindowZ); // PPACa center to window
    
        if (PpacIsHit[0] && PpacIsHit[1]) hTargetXY->Fill(TargetX,TargetY);
        if (PpacIsHit[0] && PpacIsHit[1] && hit_target) hTargetXYcut->Fill(TargetX,TargetY);
        if (gate_26al) hTargetXY_26al->Fill(TargetX,TargetY);
        if (gate_26al && hit_target) hTargetXYcut_26al->Fill(TargetX,TargetY);
    */
      
      }
    
    }
    
    if (flag_nai && flag_raw){
      for (int i=0;i<10;i++){
        nai_ch[i]->Fill(fNaiE[i]);
        if (i==0) hNaIE->Fill(0.,fNaiE[0]);
        else hNaIE->Fill(i,fNaiE[i]);
      }
    }
    
    if (flag_nai && flag_detail){
      for (int i=0;i<10;i++){
        nai_cal_ch[i]->Fill(nai_calib->Calib(i,fNaiE[i]));
        if (i==0) hNaIEcal->Fill(0.,nai_calib->Calib(i,fNaiE[i]));
        else hNaIEcal->Fill(i,nai_calib->Calib(i,fNaiE[i]));
      }
    }
    
    if (flag_ssd){
      // reset arrays
      for (int SsdNum=0;SsdNum<SsdN;SsdNum++){
        SsdIsHit[SsdNum] = false;
        if (fSsdE[SsdNum] > 150. && fSsdE[SsdNum] < 4096.){
          SsdIsHit[SsdNum] = true;
        }
      }
      for (int TelNum=0;TelNum<PsdN;TelNum++){
        for (int Gain=0;Gain<2;Gain++){
	  TelescopeIsHit[Gain][TelNum] = false;
	}
      }
      if (SsdIsHit[10] && SsdIsHit[9]){
      //if (SsdIsHit[3] || SsdIsHit[2] || SsdIsHit[10] || SsdIsHit[9]){
        TelescopeIsHit[1][0] = true;
      }
      if (SsdIsHit[11] && SsdIsHit[14] ){
      //if (SsdIsHit[11] && SsdIsHit[14] ){
      //if (SsdIsHit[4] || SsdIsHit[7] || SsdIsHit[11] || SsdIsHit[14] ){
        TelescopeIsHit[1][1] = true;
      }
      if (SsdIsHit[12]){
      //if (SsdIsHit[12] && SsdIsHit[13]){
      //if (SsdIsHit[12] || SsdIsHit[13]){
      //if (SsdIsHit[5] || SsdIsHit[6] || SsdIsHit[1] || SsdIsHit[12] || SsdIsHit[13] || SsdIsHit[8]){
        TelescopeIsHit[1][2] = true;
      }
    }
    if (flag_ssd && flag_raw){
      for (int PsdNum=0;PsdNum<PsdN;PsdNum++){
        for(int strip=0; strip<32; strip++){
          hPsdStripE[PsdNum]->Fill(strip,fPsdE[PsdNum][strip]); //
          hPsdStripT[PsdNum]->Fill(strip,fPsdT[PsdNum][strip]); //
//          if (fPsdT[PsdNum][strip] > -100) hPsdStripT[PsdNum]->Fill(strip,psd_calib_t->Calib(strip+PsdNum*32,fPsdT[PsdNum][strip])); //
	  if (fPsdE[PsdNum][strip]>500.)  strip_ch[strip+PsdNum*32]->Fill(fPsdE[PsdNum][strip]);
	}
      }
      int thisiszero=0; // testing against some strange bug...
      //hSsdE->Fill(0.,fSsdE[0]); // this worked
      //hSsdE->Fill(0,fSsdE[0]); // GCC complains when it's an integer ('0') ... crazy
      for (int SsdNum=0;SsdNum<SsdN;SsdNum++){
        hSsdE->Fill(SsdNum,fSsdE[SsdNum]);
        if (SsdIsHit[SsdNum]) hSsdT->Fill(SsdNum,fSsdT[SsdNum]);
        ssd_ch[SsdNum]->Fill(fSsdE[SsdNum]);
      }
    }
    if (flag_ssd && flag_detail){
      // reset arrays
      for (int i=0;i<SsdN;i++) {
        SsdEcal[i] = 0.;
      } 
      for (int SsdNum=0;SsdNum<SsdN;SsdNum++){
        if (SsdIsHit[SsdNum]){
          SsdEcal[SsdNum] = ssd_calib->Calib(SsdNum,fSsdE[SsdNum]);
          //if (SsdEcal[SsdNum]<0.5) SsdEcal[SsdNum]=0.;
	  // TODO: debug 0
	  hSsdEcal->Fill(SsdNum,SsdEcal[SsdNum]); // GCC complains about '0' ... WHY?!
          ssd_cal_ch[SsdNum]->Fill(SsdEcal[SsdNum]);
        }
      }
      if (TelescopeIsHit[1][0]){
	hTelescopeEHigh[0]->Fill(SsdEcal[10]+SsdEcal[9]);
	hDE_EHigh[0]->Fill(SsdEcal[10],SsdEcal[9]);
      }
      if (TelescopeIsHit[1][1]){
        hTelescopeEHigh[1]->Fill(SsdEcal[11]+SsdEcal[14]);
	hDE_EHigh[1]->Fill(SsdEcal[11],SsdEcal[14]);
      }
      if (TelescopeIsHit[1][2]){
	//if (SsdEcal[12]>0.1 ||SsdEcal[12]>0.1 || SsdEcal[8]>0.1) hTelescopeEHigh[2]->Fill(SsdEcal[12]+SsdEcal[13]+SsdEcal[8]);
	hTelescopeEHigh[2]->Fill(SsdEcal[12]+SsdEcal[13]+SsdEcal[8]);
	if (SsdEcal[12]+SsdEcal[13]+SsdEcal[8]>1.4)  hTelescopeEHighcut[2]->Fill(SsdEcal[12]+SsdEcal[13]+SsdEcal[8]);
	hDE_EHigh[2]->Fill(SsdEcal[12],SsdEcal[13]);
      }
      
      //if (SsdIsHit[1] && SsdIsHit[2] && !PpacIsHit[0] && !PpacIsHit[1]){
          //  if (SsdEcal[1]+SsdEcal[2]>=0.9) ssd_cal_ch[3]->Fill(SsdEcal[1]+SsdEcal[2]);
      //}
      
      if (fNstdc>0.1 && !PpacIsHit[0] && !PpacIsHit[1]) {
        //if (fNstdc>11.9) fNstdc-=12.;
        hNstdc->Fill(fNstdc);
      }
    
      // reset arrays
      for(int PsdNum=0; PsdNum<PsdN; PsdNum++){ 
        PsdEcal[PsdNum] = 0.;
        PsdTcal[PsdNum] = -1000;
        PsdEmax[PsdNum] = 0.;
        for(int PsdXY=0; PsdXY<2; PsdXY++){ 
          for(int strip=0; strip<16; strip++){
            PsdE_XY_cal[PsdNum][PsdXY][strip] = 0.;
            PsdE_XY_max[PsdNum][PsdXY] = 0.;
            PsdEmax_ch[PsdNum][PsdXY] = -1;
            PsdIsHit[PsdNum][PsdXY] = false;
          }
        }
      }
   
      for(int ssd=0;ssd<SsdN;ssd++){
        
      }
      
      if (flag_ppac) {
        for (int rf_num=0;rf_num<2;rf_num++){
          if (PpacIsHit[2] && SsdIsHit[0])
          hF2SsdRf[rf_num]->Fill(fRf[rf_num],SsdEcal[0]);
        }
      }
      
      for(int PsdNum=0; PsdNum<PsdN; PsdNum++){
        for(int strip=0; strip<16; strip++){
        // checked up to here for double spacing on brackets etc
          for (int PsdXY=0; PsdXY<2; PsdXY++){
            // PSD0_Calib[strip] = psd_calib->Calib(strip,fPsdE[0][strip]);
            // CAMAC maximum is (15/16)*4096 as the last 1/16 is not used for actual data
            if (fPsdE[PsdNum][strip+(PsdXY*16)]>200. && fPsdE[PsdNum][strip+(PsdXY*16)]<3841.){
              PsdE_XY_cal[PsdNum][PsdXY][strip] = psd_calib->Calib(strip+(PsdXY*16)+PsdNum*32,fPsdE[PsdNum][strip+(PsdXY*16)]);
            }
    
            if (PsdE_XY_cal[PsdNum][PsdXY][strip]>PsdE_XY_max[PsdNum][PsdXY]) {//find maximum strip
              PsdE_XY_max[PsdNum][PsdXY]=PsdE_XY_cal[PsdNum][PsdXY][strip];
              PsdEmax_ch[PsdNum][PsdXY]=strip; 
              PsdIsHit[PsdNum][PsdXY]=true;
            }
	  }
        }
        if (PsdIsHit[PsdNum][0] && PsdIsHit[PsdNum][1]) {
//	    cout <<  PsdEmax_ch[PsdNum][0] << " " << PsdEmax_ch[PsdNum][1]<< ":" << (PsdEmax_ch[PsdNum][0]) << " " << (PsdEmax_ch[PsdNum][1]) << " " ;
          ////cout << PsdEmax_ch[0][0] << "\t" << PsdEmax_ch[0][1] << endl;
          //strip_cal_ch[PsdEmax_ch[PsdNum][0]+PsdNum*32]->Fill(PsdE_XY_max[PsdNum][0]);
          //strip_cal_ch[PsdEmax_ch[PsdNum][1]+16+PsdNum*32]->Fill(PsdE_XY_max[PsdNum][1]);
          hPsdStripEcal[PsdNum]->Fill(PsdEmax_ch[PsdNum][0],PsdE_XY_max[PsdNum][0]);
          hPsdStripEcal[PsdNum]->Fill(PsdEmax_ch[PsdNum][1]+16,PsdE_XY_max[PsdNum][1]);
//          if (PsdNum==2) cout << PsdEmax_ch[PsdNum][0] << ": " << fPsdT[PsdNum][0] <<"\t" << fPsdT[PsdNum][1] << "\t" <<fPsdT[PsdNum][2] << "\t" <<fPsdT[PsdNum][3] << "\t" <<fPsdT[PsdNum][4] << "\t" <<fPsdT[PsdNum][5] << "\t" <<fPsdT[PsdNum][6] << "\t" <<fPsdT[PsdNum][7] << "\t" <<fPsdT[PsdNum][8] << "\t" <<fPsdT[PsdNum][9] << "\t" <<fPsdT[PsdNum][10] << "\t" <<fPsdT[PsdNum][11] << "\t" <<fPsdT[PsdNum][12] << "\t" <<fPsdT[PsdNum][13] << "\t" <<fPsdT[PsdNum][14] << "\t" <<fPsdT[PsdNum][15] << endl;
/*
if (PsdNum==2){
            if ( fPsdT[PsdNum][PsdEmax_ch[PsdNum][0]] > 0) {}
          else{
cout << PsdEmax_ch[PsdNum][0] << ": " << fPsdT[PsdNum][0] <<"\t" << fPsdT[PsdNum][1] << "\t" <<fPsdT[PsdNum][2] << "\t" <<fPsdT[PsdNum][3] << "\t" <<fPsdT[PsdNum][4] << "\t" <<fPsdT[PsdNum][5] << "\t" <<fPsdT[PsdNum][6] << "\t" <<fPsdT[PsdNum][7] << "\t" <<fPsdT[PsdNum][8] << "\t" <<fPsdT[PsdNum][9] << "\t" <<fPsdT[PsdNum][10] << "\t" <<fPsdT[PsdNum][11] << "\t" <<fPsdT[PsdNum][12] << "\t" <<fPsdT[PsdNum][13] << "\t" <<fPsdT[PsdNum][14] << "\t" <<fPsdT[PsdNum][15] << endl;}
	  }*/
          //if (PsdNum==2){
          //  if ( fPsdT[PsdNum][PsdEmax_ch[PsdNum][0]] > 0) cout << ".";
	  //  else cout << "!" ;
	  //}
	  PsdEmax[PsdNum] = (PsdE_XY_max[PsdNum][0] + PsdE_XY_max[PsdNum][1])/2.;

//from raw data part 08 Jun 2017 12:04:42 
//          if (fPsdT[PsdNum][strip] > -100) hPsdStripT[PsdNum]->Fill(strip,psd_calib_t->Calib(strip+PsdNum*32,fPsdT[PsdNum][strip])); //

	  PsdTcal[PsdNum] = psd_calib_t->Calib(PsdEmax_ch[PsdNum][0]+PsdNum*32,fPsdT[PsdNum][PsdEmax_ch[PsdNum][0]]);
//    	  if (PsdNum==2) cout << "X: " << PsdE_XY_max[2][0] << "\tY: " << PsdE_XY_max[2][1] << "\tA: " << PsdEmax[2] 
//	  << "\tT: " << PsdTcal[2] << "\tCH: " << PsdEmax_ch[PsdNum][0]<< endl;
          hPsdStripTcal[PsdNum]->Fill(PsdEmax_ch[PsdNum][0],PsdTcal[PsdNum]);
//PsdTcal[PsdNum] = fPsdT[PsdNum][PsdEmax_ch[PsdNum][0]];
	  //cout << PsdE_XY_max[PsdNum][0] << ":" << PsdE_XY_max[PsdNum][1] << "\t" ;
	  if ( PsdEmax_ch[PsdNum][0] !=-1 && PsdEmax_ch[PsdNum][1] !=-1 ) 
	    hPsdXY[PsdNum]->Fill(15-PsdEmax_ch[PsdNum][0],(15-PsdEmax_ch[PsdNum][1]));
        }
        for (int PsdNum=0;PsdNum<PsdN;PsdNum++){
          for(int strip=0; strip<32; strip++){
            if (fPsdE[PsdNum][strip]>300.) strip_cal_ch[strip+PsdNum*32]->Fill(psd_calib->Calib(strip+PsdNum*32,fPsdE[PsdNum][strip]));
          }
        }
      }
      if (PsdIsHit[1][0] && PsdIsHit[1][1] && SsdIsHit[4]){
        TelescopeIsHit[0][1] = true;
      }
      if (PsdIsHit[2][0] && PsdIsHit[2][1] && SsdIsHit[5]){
        TelescopeIsHit[0][2] = true;
      }
      if (TelescopeIsHit[0][1] && gate_26al && fCr[0]){ 
        hTelescopeELow[1]->Fill(PsdEmax[1]+SsdEcal[4]+SsdEcal[7]);
	hDE_ELow[1]->Fill(PsdEmax[1],PsdEmax[1]+SsdEcal[4]+SsdEcal[7]);
    	hTelescopeE_Tof[1]->Fill(PsdEmax[1]+SsdEcal[4]+SsdEcal[7],tof[0]-PsdTcal[1]);
    	//hTelescopeE_Tof[1]->Fill(PsdEmax[1]+SsdEcal[4]+SsdEcal[7],fSsdT[4]-tof[0]);
        if (
	    cut_de_e_proton_tel2->IsInside(PsdEmax[1],PsdEmax[1]+SsdEcal[4]+SsdEcal[7])
	    ){
          //hPsdStripT[1]->Fill(PsdEmax_ch[1][0],fPsdT[1][PsdEmax_ch[1][0]]); //
	  hProton[1]->Fill(PsdEmax[1]+SsdEcal[4]+SsdEcal[7]);
    	  hTelescopeE_Tofcut[1]->Fill(PsdEmax[1]+SsdEcal[4]+SsdEcal[7],tof[0]-PsdTcal[1]); 
    	  //hTelescopeE_Tofcut[1]->Fill(PsdEmax[1]+SsdEcal[4]+SsdEcal[7],fSsdT[4]-tof[0]); 
    	  //hProton->Fill(PsdE_XY_max[2][1]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
	    }
      }
      // some additional work with PsdNum was done up to here 25 Feb 2017 20:00:12 
      if (TelescopeIsHit[0][2] && gate_26al && fCr[0]) 
      //if (TelescopeIsHit[0][2]) 
      //if (PsdIsHit[2][0] && SsdIsHit[5] ) 
      //if (PsdIsHit[2][0] && SsdIsHit[5] && gate_26al) 
      //if (PsdIsHit[0][0] && PsdIsHit[0][1] && SsdIsHit[1] && gate_26al)
      {
        hTelescopeELow[2]->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
	hDE_ELow[2]->Fill(PsdEmax[2],PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
    	hTelescopeE_Tof[2]->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],PsdTcal[2]-tof[0]);
    	//hTelescopeE_Tof[2]->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],fSsdT[5]-tof[0]);
        //dE_E[0]->Fill(PsdE_XY_max[0][0],SsdEcal[1]);
	//dE_E[2]->Fill(PsdEmax[2],SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
	//dE_E[2]->Fill(PsdE_XY_max[2][1],SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
        if (cut_de_e_proton_tel2->IsInside(PsdEmax[2],PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1])  
	   &&  cut_e_tof_tel2->IsInside(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],PsdTcal[2]-tof[0]) 
	   // &&  cut_e_tof_tel2->IsInside(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],fSsdT[5]-tof[0]) 
	   )
	{
	  hProton[2]->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
//	  hDE_ELowcut[2]->Fill(PsdEmax[2],PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
          //if (fPsdT[2][PsdEmax_ch[2][0]]>-500) {
	  //  hPsdStripT[2]->Fill(PsdEmax_ch[2][0],fPsdT[2][PsdEmax_ch[2][0]]); //
          //  hPsdStripTcal[2]->Fill(PsdEmax_ch[2][0],PsdTcal[2]);
	  //}
    	  //hProton->Fill(PsdE_XY_max[2][1]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
//	  hTelescopeE_Tofcut[2]->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],PsdTcal[2]-tof[0]);
	    //cout << "\t" << tof[0]-PsdTcal[2] << endl;
	  //cout << "\t" << tof[0]-PsdTcal[2] << endl;
    	  //hTelescopeE_Tofcut[2]->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],fSsdT[5]-tof[0]); 
    	  //hTelescopeE_Tofcut->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],PsdTcal[2]-tof[0]); 
    	  //hTelescopeE_Tofcut->Fill(PsdE_XY_max[2][1]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],tof[0]-fSsdT[5]); 
    	
    	}
        if (cut_de_e_deuteron_tel2->IsInside(PsdEmax[2],PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1])  
	   &&  cut_e_tof_d_tel2->IsInside(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],PsdTcal[2]-tof[0]) 
	   // &&  cut_e_tof_tel2->IsInside(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],fSsdT[5]-tof[0]) 
	   )
	{
	  hDeuteron[2]->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
	  hDE_ELowcut[2]->Fill(PsdEmax[2],PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1]);
	  hTelescopeE_Tofcut[2]->Fill(PsdEmax[2]+SsdEcal[5]+SsdEcal[6]+SsdEcal[1],PsdTcal[2]-tof[0]);
	}
      }
    }

    
    		//cout << fPsdE[0][PSD0_ChF] <<endl;
    
    		//PSD2_Y_Calib[PSD1_ChB] = slope_Y[PSD1_ChB-16]*fPsdE[1][PSD1_ChB]+offset_Y[PSD1_ChB-16];
    
    		//cout << PSD1_X_Calib[PSD0_ChF] << "\t" << PSD2_X_Calib[PSD1_ChF] << endl;
    
  }
}

