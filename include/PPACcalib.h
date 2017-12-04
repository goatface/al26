#ifndef PPACCALIB_H
#define PPACCALIB_H
#include "TROOT.h"

static Double_t PpacOffset(Int_t PPACnum, Int_t axis){
    Double_t ppacoffset[3][2];

    ppacoffset[0][0] = 0.92;
    ppacoffset[0][1] = 1.58;

    ppacoffset[1][0] = 0.05;
    ppacoffset[1][1] = 0.04;

    ppacoffset[2][0] = 0.05;
    ppacoffset[2][1] = 0.04;

    return ppacoffset[PPACnum][axis];
}


static Double_t PpacOffsetLine(Int_t PPACnum, Int_t axis){
    Double_t ppacoffsetline[3][2];

    ppacoffsetline[0][0] = 0.0;
    ppacoffsetline[0][1] = -0.22;
    ppacoffsetline[1][0] = -0.8296;
    ppacoffsetline[1][1] = -1.0;
    ppacoffsetline[2][0] = 0.759;
    ppacoffsetline[2][1] = -2.28;

    return ppacoffsetline[PPACnum][axis];

}

static Double_t PpacOffsetGeometry(Int_t PPACnum, Int_t axis){
    Double_t ppacoffsetgeometry[3][2];

    ppacoffsetgeometry[0][0] = -3.0;
    ppacoffsetgeometry[0][1] = -1.0;
    ppacoffsetgeometry[1][0] = 0.0;
    ppacoffsetgeometry[1][1] = 0.0;
    ppacoffsetgeometry[2][0] = 0.0;
    ppacoffsetgeometry[2][1] = 0.0;

    return ppacoffsetgeometry[PPACnum][axis];

}

static Double_t PpacPositionGain(Int_t PPACnum, Int_t axis){
    Double_t ppacpositiongain[3][2];

    ppacpositiongain[0][0] = 0.6200;
    ppacpositiongain[0][1] = 0.6210;
    ppacpositiongain[1][0] = 0.6285;
    ppacpositiongain[1][1] = 0.6285;
    ppacpositiongain[2][0] = 0.6285;
    ppacpositiongain[2][1] = 0.6285;

    return ppacpositiongain[PPACnum][axis];

}
#endif //PPACCALIB_H
