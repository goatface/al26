#ifndef FIT_H
#define FIT_H
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TObject.h"
#include <TH1D.h>
#include <TH1F.h>
#include <TF1.h>
#include <TGraph.h>
#include <math.h>
#include <TH2D.h>
#include <string>
#include <vector>
#include <iostream>
#include "TApplication.h"

using namespace::std;

class fit : public TObject {

  private :

  public :
    fit(){};
    ~fit(){
    }

    void FitGauss2(TH1F *histo);
    void FitGauss(TH1D *hist);

    ClassDef(fit,1);
};

#endif
