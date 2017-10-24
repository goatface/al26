/*
 Copyright: 
 2011, 2012, 2013 daid KAHL

 This file is part of crabat

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
   
 In order to publish results obtained from crabat or any
 future derivative software, the authors must make reference
 to the paper describing it (in preparation): D. Kahl et al.
 ``Algorithms for Active Target Data Analysis'' 2013.  This is an 
 additional term to the license as stipulated in section 7b.
 Publication includes, but is not limited to: books, peer-
 reviewed journals, conference proceedings, annual reports, etc.
 
*/

/**
@file Analyzer_config.cxx
@author daid
@version 1.0
 * @brief Sets configurations which vary experiment-by-experiment
 * @details If we share analysis code across experiments,\n
 * it is impractical to get new updates which over-ride experiment\n
 * specific data.  But if we put these details all only in one file\n
 * it is much easier for different experiments to use the crabat code.\n
 * Handles MSTPC pad mapping.\n
 * Handles MSTPC pad gain.\n
 * Does detailed PPAC calibration.\n
 * Defines rf frequency.
@date 05 Dec 2011 16:03:48  
*/
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

#define Analyzer_config_cxx


UShort_t inline Analyzer::SetPpacSepZ()
{
  return(440.) ; // mm p. 195 16 Sep 2016 19:29:52 
}

// change this to pass by address
Bool_t inline Analyzer::WindowCut(Double_t x, Double_t y)
{
// modified from http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=1145
  double radius = 20; // 26Al 
  const Int_t n = 300; 
  Double_t winx[n+1],winy[n+1]; 
  Double_t rcut = 1.00*radius; 
  Double_t dphi = TMath::TwoPi()/n; 
  for (Int_t i=0;i<n;i++) { 
    winx[i] = rcut*TMath::Cos(i*dphi); 
    winy[i] = rcut*TMath::Sin(i*dphi); 
  } 
  winx[n] = winx[0]; winy[n] = winy[0]; 
  TCutG *window = new TCutG("window",n+1,winx,winy); 
  if (window->IsInside(x,y)) {
    delete window;
    return(true);
  }
  delete window;
  return (false);
}

// 31 May 2017 15:50:17 
// moving to another header so it can be shared with root macros
// being part of the "Analyzer" class is pointless in fact
//Float_t inline Analyzer::RfShift(Int_t run, Int_t rf){
//  if (rf!=0 && rf!=1){
//    cerr << "\t\tRF was not 0 or 1!" << endl;
//    return(-1000.);
//  }
///*
//
//TODO: Add decay runs!
//
//Set		Runs
//0		46~70
//1		81~99
//2		102~130
//3		136~141
//4		142~181
//5		187~234
//        === RF0 ===
//From macros/rf_shift.C
//        Set     Peak 0          Peak 1          Delta           Shift
//        0       42.9935         102.322         59.3282         -24.9146
//        1       18.0788         79.361          61.2821         0
//        2       7.90395         68.997          61.093          10.1749
//        3       17.7335         79.0016         61.2682         0.345369
//        4       24.1699         85.4664         61.2966         -6.09103
//        5       14.3485         75.5473         61.1989         3.73036
//        
//        Average:                                61.2277
//        
//        === RF1 ===
//        Set     Peak 0          Peak 1          Delta           Shift
//        0       31.9655         93.0474         61.0819         -24.1983
//        1       7.76724         68.2702         60.503          0
//        2       -0.53215        57.9569         58.489          8.29939
//        3       7.48953         68.0005         60.511          0.277708
//        4       13.5623         74.4096         60.8473         -5.79508
//        5       4.4102          64.5308         60.1206         3.35704
//        
//        Average:                                60.6128
//*/
//  Float_t rf_shift[2][6]={
//  {  
//  -24.9146,
//  0.,
//  10.1749,
//  0.345369,
//  -6.09103,
//  3.73036,
//  },{  
//  -24.1983,
//  0.,
//  8.29939,
//  0.277708,
//  -5.79508,
//  3.35704
//  }};
//  
//  if (run>=46 && run<=70){
//    return (rf_shift[rf][0]);
//  }
//  else if (run>=81 && run<=99){
//    return (rf_shift[rf][1]);
//  }
//  else if (run>=102 && run<=130){
//    return (rf_shift[rf][2]);
//  }
//  else if (run>=136 && run<=141){
//    return (rf_shift[rf][3]);
//  }
//  else if (run>=142 && run<=181){
//    return (rf_shift[rf][4]);
//  }
//  else if (run>=187 && run<=234){
//    return (rf_shift[rf][5]);
//  }
//  else {
//    cerr << "\t\tNo RF shift for run " << run << endl;
//    return(0.);
//  }
//}

UShort_t inline Analyzer::PpacXYCalib(Bool_t flag_detail, Bool_t flag_ppac, Int_t run){ 
  if (!flag_detail || !flag_ppac) return(EXIT_FAILURE);
  // PPAC calibration that is not handled in ppac_calib.dat
  Float_t dxy[2]; // 1 is X, 2 is Y; tmp variable for calibration loops
  Float_t PpacOffset[3][2];
  Float_t PpacOffsetLine[3][2];
  Float_t PpacPositionGain[3][2];
  Float_t PpacOffsetGeometry[3][2];

//02 Mar 2017 16:06:22 checked.  Double-check sign on geometric offsets

  PpacOffset[0][0]=0.92;
  PpacOffset[0][1]=1.58;
  PpacOffsetLine[0][0]=0.0;
  PpacOffsetLine[0][1]=-0.22;
  PpacOffsetGeometry[0][0]=-2.3;
  PpacOffsetGeometry[0][1]=0.9;
  PpacPositionGain[0][0]=0.6200;
  PpacPositionGain[0][1]=0.6210;

  PpacOffset[1][0]=0.03;
  PpacOffset[1][1]=0.03;
  PpacOffsetLine[1][0]=-0.8296;
  PpacOffsetLine[1][1]=-1.0;
  // checked 18 May 2017 12:51:35 
  if (run<165){
    PpacOffsetGeometry[1][0]=0.0;
  }else if (run>164 && run<175){
    PpacOffsetGeometry[1][0]=-15.0;
  }else if (run>174){
    PpacOffsetGeometry[1][0]=-25.0;
  }
  PpacOffsetGeometry[1][1]=-0.8;
  PpacPositionGain[1][0]=0.6285;
  PpacPositionGain[1][1]=0.6285;
  
  PpacOffset[2][0]=0.03;
  PpacOffset[2][1]=0.03;
  PpacOffsetLine[2][0]=0.759;
  PpacOffsetLine[2][1]=-2.28;
  PpacOffsetGeometry[2][0]=0.0;
  PpacOffsetGeometry[2][1]=0.0;
  PpacPositionGain[2][0]=0.6285;
  PpacPositionGain[2][1]=0.6285;

  // PPAC calibration for loop
  for (UShort_t i=0;i<3;i++){
    //average time
    tof[i] = 0.25*(fPpac[i][0]+fPpac[i][1]+fPpac[i][2]+fPpac[i][3]);
    dxy[0]=(fPpac[i][0]-fPpac[i][1]);
    dxy[1]=(fPpac[i][2]-fPpac[i][3]);
    for (UShort_t ii = 0; ii < 2; ii++) {
      // time calibration
      dxy[ii] -=PpacOffset[i][ii];
      dxy[ii] -= PpacOffsetLine[i][ii];
      // position calibration
      dxy[ii] *= PpacPositionGain[i][ii];
      dxy[ii] -= PpacOffsetGeometry[i][ii];
    }
    PpacX[i]=dxy[0];
    PpacY[i]=dxy[1];
  } //end PPAC calibration for loop
  return(EXIT_SUCCESS);
}


