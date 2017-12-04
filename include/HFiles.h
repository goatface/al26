#ifndef HFILES_H
#define HFILES_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <vector>
using namespace::std;

class HFiles{

    private:
        ifstream file;
        string buf;
        vector<string> split(string& input, char delimiter);
        inline void DeleteSpace(string &buffer);

        vector<int> Dim;

        vector<string> setup;
        vector<string> axes;

    public:
        HFiles();
        HFiles(string filename){
          OpenFile(filename);
        }
        ~HFiles();
        bool OpenFile(string filename);
        void CloseFile(){
            file.close();
        };

        bool ReadNumber(int Number);

        string ReadEquality(string LeftHand);
        vector<string> ReadID();
        int ReadDim(int i);
        vector<string> ReadSetup(int i);
        vector<string> ReadAxes(int i);
        vector<string> ReadCSV();
        vector<string> ReadData(int NofData);

        void PutData(string data);

};


#endif //HFILES_H
