#include <TCalibration.h>

TCalibration::TCalibration(string Detector, int runNum){

    if(Detector=="SSD"){
        HFiles* file = new HFiles();
        file->OpenFile("calib/SSDcalib.dat");
        vector<string> data = file->ReadCSV();
        for(int i=0; i<data.size(); i++){
            if(i%2==0){
                SSDgain[i/2] = atof(data.at(i).c_str());
            }else{
                SSDoffset[(i-1)/2] = atof(data.at(i).c_str());
            }
        }
       file->CloseFile();
        delete file;
        HFiles* fileEth = new HFiles();
        fileEth->OpenFile("calib/SsdEth.dat");
        vector<string> Eth = fileEth->ReadCSV();
        for(int j = 0; j < Eth.size(); j++)
        {
            SsdEth[j] = atof(Eth.at(j).c_str());
        }
        delete fileEth;
    }else if(Detector=="PSD"){

        int fPsdN = 0;

        while(fPsdN<3){
            char DatName[30];
            sprintf(DatName,"calib/PSD%dcalib.dat",fPsdN);
            HFiles* file = new HFiles();
            file->OpenFile(DatName);
            vector<string> data = file->ReadCSV();
            for(int i=0; i<data.size(); i++){
                if(i%2==0){
                    PSDgain[fPsdN][i/2] = atof(data.at(i).c_str());
                }else{
                    PSDoffset[fPsdN][(i-1)/2] = atof(data.at(i).c_str());
                }
            }
            file->CloseFile();
            delete file;
            fPsdN++;
        }

    }else if(Detector=="NaI"){
        HFiles* file = new HFiles();
        file->OpenFile("calib/NaIcalib.dat");
        vector<string> data = file->ReadCSV();
        for(int i=0; i<data.size(); i++){
            if(i%2==0){
                NaIgain[i/2] = atof(data.at(i).c_str());
            }else{
                NaIoffset[(i-1)/2] = atof(data.at(i).c_str());
            }
        }
        file->CloseFile();
        delete file;

        HFiles* file2 = new HFiles();
        file2->OpenFile("calib/NaIGainShift.dat");
        vector<string> gainShift;
        if(file2->ReadNumber(runNum)){
            gainShift = file2->ReadData(10);
        }else{
//            cout << "NaI Gain Shift didn't match" << endl;
        }
        for(int i=0; i<gainShift.size(); i++){
            if(atof(gainShift.at(i).c_str()))
              NaIGainShift[i] = atof(gainShift.at(i).c_str());
            else
              NaIGainShift[i] = 1.0;
//            cout << "NaIGainShift[" << i << "] = " << NaIGainShift[i] << endl;
        }
        file2->CloseFile();
        delete file2;

    }else{
        cerr << "Error: Detector " << Detector <<  " doesn't exist in calibration " << endl;
        exit(1);
    }

}
TCalibration::~TCalibration(){

}

void TCalibration::PSDcalib_write(double* gain, double* offset, int psd){

    FILE* CalibData;
    char filename[100],Comment[40];
    sprintf(filename,"calib/PSD%dcalib.dat",psd);
    sprintf(Comment,"#### PSD%d gain, offset #### \n",psd);
    CalibData = fopen(filename,"w");
    fputs(Comment,CalibData);

    char Parameters[100];

    for(int ch=0; ch < 32; ch++)
    {
        sprintf(Parameters,"%.5f , %.5f;\n", gain[ch], offset[ch]);
        fputs(Parameters,CalibData);
    }
    fclose(CalibData);

}


void TCalibration::SSDcalib_write(double gain, double offset, int ssd){

    FILE* CalibData;
    CalibData = fopen("calib/SSDcalib.dat", "a");
    char Parameters[100],Comment[40];
    sprintf(Parameters,"%.5f , %.5f;\n", gain, offset);
    sprintf(Comment,"#### SSD%d gain, offset #### Sigma=%f \n",ssd, CalibSigma);
    fputs(Comment,CalibData);
    fputs(Parameters,CalibData);
    fputs("#\n",CalibData);
    fclose(CalibData);

}


void TCalibration::Alpha3Calib(TH1D* hist, Int_t bin, Int_t hMin, Int_t hMax){

    TCanvas* canv = new TCanvas("SSD calib");
    canv->Divide(2,1);
    canv->cd(1);
    hist->Draw();

//    cout << "loop " << count[0] << endl;
    Double_t sigma[3];

    Int_t maxpeaks = 3;
    TSpectrum *s = new TSpectrum(maxpeaks);
    s->Search(hist, 1 ,"new",0.1);
    Double_t *x = s->GetPositionX();

    cout << x[0] << "," << x[1] <<  " , " << x[2] << endl;

    Int_t binN[3];
    Int_t h[3];
    for(Int_t i=0;i<3;i++){
    binN[i] = Int_t(x[i]/hMax*bin)+1;

    cout << i+1 << " peak is at "  << binN[i] << " bin " << endl;

    h[i] = hist->GetBinContent(binN[i]);

    cout << " Peak height " << h[i] << endl;
    }

    Int_t fh[3],binNh[3];
    for(Int_t i=0;i<3;i++){
        binNh[i]=binN[i];
        while((Double_t)fh[i]<(Double_t)h[i]*0.5){
            fh[i] = hist->GetBinContent(binNh[i]);
            binNh[i]+=5;
        }
        sigma[i] = (binNh[i]-binN[i])*2;
   }

    TF1 *fit1 = new TF1("fit1", "[0]*exp(-0.5*((x-[1])/[2])**2)",x[0]-4*sigma[0],x[0]+4*sigma[0]);
    TF1 *fit2 = new TF1("fit2", "[0]*exp(-0.5*((x-[1])/[2])**2)",x[1]-4*sigma[1],x[1]+4*sigma[1]);
    TF1 *fit3 = new TF1("fit3", "[0]*exp(-0.5*((x-[1])/[2])**2)",x[2]-4*sigma[2],x[2]+4*sigma[2]);

    fit1->SetParameters(h[0], x[0], sigma[0]);
    fit2->SetParameters(h[1], x[1], sigma[1]);
    fit3->SetParameters(h[2], x[2], sigma[2]);
    fit1->SetLineColor(7);
    fit2->SetLineColor(7);
    fit3->SetLineColor(7);
    hist->Fit("fit1","same","",x[0]-4*sigma[0],x[0]+4*sigma[0]);
    hist->Fit("fit2","same","",x[1]-4*sigma[1],x[1]+4*sigma[1]);
    hist->Fit("fit3","same","",x[2]-4*sigma[2],x[2]+4*sigma[2]);


    // Tipcal alpha source energy
    // 1st 3.148MeV
    // or 4.780MeV
    // 2nd 5.480MeV
    // 3rd 5.795MeV

    Double_t tmp, p1[3], a_3[3];
    Double_t a,b,fSigma;




    HFiles* DataFile = new HFiles();
    DataFile->OpenFile("data/calib/AlphaSource.dat");

//    a_3[0]=3.148; // for alpha-3
//        a_3[0]=4.780; //for alpha-2
//        a_3[1]=5.488;
//        a_3[2]=5.795;

    a_3[0] = atof(DataFile->ReadEquality("Peak1").c_str());
    a_3[1] = atof(DataFile->ReadEquality("Peak2").c_str());
    a_3[2] = atof(DataFile->ReadEquality("Peak3").c_str());
    delete DataFile;


    p1[0] = fit1->GetParameter(1);
    p1[1] = fit2->GetParameter(1);
    p1[2] = fit3->GetParameter(1);

/* Sort */
    for (Int_t i=0; i<3; ++i) {
        for (Int_t j=i+1; j<3; ++j) {
            if (p1[i] > p1[j]) {
                tmp =  p1[i];
                p1[i] = p1[j];
                p1[j] = tmp;
            }
        }
    }

    LSM(3,p1,a_3,&a,&b,&fSigma);

    cout << "p1[0] = " << p1[0] << endl;
    cout << "p1[1] = " << p1[1] << endl;
    cout << "p1[2] = " << p1[2] << endl;

    cout << "f(x)=" << a << "x + " << b << endl;
    cout << "sigma^2 = " << fSigma << endl;
    cout << " Energy = "<< a << "*(Channel - (" << -b/a << "))" << endl;

    GainCalib = a;
    OffsetCalib = b/a;
    CalibSigma = sqrt(fSigma);



/*      For Graph      */
/*   Not necessary    */

    canv->cd(2);

    Int_t PLOT = 100;
    Double_t u[PLOT],v[PLOT];

    for(Int_t i=0; i<PLOT;i++){
        u[i] = (Double_t)i/PLOT*p1[2]*1.5;
        v[i] = a*u[i]+b;
    }

    TGraph *gGraph = new TGraph(PLOT,u,v);
    gGraph->GetXaxis()->SetTitle("Channel");
    gGraph->GetYaxis()->SetTitle("Energy[keV]");
    gGraph->SetName(0);
    gGraph->GetXaxis()->SetLabelSize(0.03);
    gGraph->GetXaxis()->SetLabelFont(22);
    gGraph->GetXaxis()->SetTitleFont(22);
    gGraph->GetXaxis()->CenterTitle();
    gGraph->GetYaxis()->CenterTitle();
    gGraph->GetYaxis()->SetTitleFont(22);
    gGraph->GetYaxis()->SetLabelSize(0.03);
    gGraph->GetYaxis()->SetLabelFont(22);
    gGraph->SetMarkerColor(kBlack);
    gGraph->SetTitle(0);
    gGraph->Draw("AL");

    TGraph *gData = new TGraph(3,p1,a_3);
    gData->SetMarkerColor(kRed);
    gData->SetMarkerStyle(20);
    gData->SetMarkerSize(1.0);
    gData->Draw("SAME P");

    Char_t formula[100];
    Char_t error[100];

    sprintf(formula,"f(x)=%.3fx%+.2f",a,b);
    sprintf(error,"#sigma^{2}=%.2f",fSigma);
    cout << " Energy = "<< a << "(Channel - (" << -b/a << "))" << endl;


    TLatex *txt = new TLatex();
    txt->SetTextSize(0.04);
    txt->SetTextAlign(21);
    txt->SetTextFont(22);
    txt->DrawLatex((Double_t)3/4*u[PLOT-1],v[PLOT-1],formula);
    txt->DrawLatex((Double_t)3/4*u[PLOT-1],0.95*v[PLOT-1],error);


}

void TCalibration::LSM(Int_t N, Double_t *u, Double_t *v, Double_t *a, Double_t *b, Double_t *fSigma){

    Double_t p0=0.0, p1=0.0, p2=0.0, p3=0.0, s=0.0;

    for(Int_t i=0; i<N; i++){
        p0 += u[i];
        p1 += v[i];
        p2 += u[i] * v[i];
        p3 += pow(u[i],2);
    }

    *a = (N*p2-p0*p1)/(N*p3-pow(p0,2));
    *b = (p3*p1-p2*p0)/(N*p3-pow(p0,2));

    for(Int_t i=0; i<N; i++){
        s += pow(v[i]-u[i]*(*a)-*b,2)/N;
    }
    *fSigma = s;

}
