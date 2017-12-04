#ifndef HCANVAS_H
#define HCANVAS_H
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TObject.h"
#include <TH1D.h>
#include <TH1D.h>
#include <TH2D.h>
#include <string>
#include <iostream>
#include "TApplication.h"

using namespace::std;

class canv : public TCanvas {

  private :
    int nHist;
    TH1D* hHist1D[10];
    TH2D* hHist2D[10];
    char hTitle[10][100];

    bool hSet1D;
    bool hSet2D;

    enum sort{

      HIST_1D = 0,
      HIST_2D = 1,
      GRAPH = 2,

    };

    sort SortObject[10];

    void SetHist1D(TH1D* hist);
    void SetHist2D(TH2D* hist);

  public :
    canv();
    ~canv(){
    }
    void hDraw(){
      cout << "canv loaded" << endl;
    }

    void Set(TH1D* hist){SetHist1D(hist);};
    void Set(TH2D* hist){SetHist2D(hist);};


    TH1D* CombineHist(TH1D* hist1, TH1D* hist2);
    void CompareHist(TH1D* hist1, TH1D* hist2);
//    void DrawSame();
    void DrawAll();
    void MultiDraw();

    ClassDef(canv,1);
};


#endif //HCANVAS_H
