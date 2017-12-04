#include <HFiles.h>

HFiles::HFiles(){

}

HFiles::~HFiles(){

}

vector<string> HFiles::split(string& input, char delimiter)
{
    istringstream stream(input);
    string field;
    vector<string> result;
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }
    return result;
}

inline void HFiles::DeleteSpace(string &buffer){
    size_t pos;
    while((pos = buffer.find_first_of(" 　\t")) != string::npos){
        buffer.erase(pos, 1);
    }
}


bool HFiles::OpenFile(string filename){

    file.open(filename.c_str());
    if(!file || !file.is_open() || file.bad() || file.fail()){
        cout << "Invalid file specified" << endl;
        return false;
    }
    return 0;
}


bool HFiles::ReadNumber(int Number){

    file.seekg(0,ios_base::beg);

    while(getline(file,buf))
    {
        if(buf[0]=='$'){
            DeleteSpace(buf);
//            buf.erase(buf.begin());
//            cout << buf << endl;
//            cout << " Read Number " << buf << endl;
            int n = atoi(buf.c_str());
            if(n==Number)
                return true;
        }
    }
    return false;

}


string HFiles::ReadEquality(string LeftHand){

    file.seekg(0,ios_base::beg);
    while(getline(file,buf)){
        if(buf[0]=='#')
            continue;
        if(buf.empty())
            continue;
        DeleteSpace(buf);
        vector<string> strvec = split(buf,'=');
        for(int i=0; i<strvec.size(); i++){
            if(strvec.at(i)==LeftHand)
                return strvec.at(i+1);
        }
    }
    cerr << "Error: "  << LeftHand << " Not Found " <<  endl;
    return 0;

}

vector<string> HFiles::ReadID(){

    vector<string> data;
    while(getline(file,buf)){
        if(buf[0]=='#')
            continue;
        if(buf.empty())
            continue;
        vector<string> ID_setup = split(buf,'|');
        vector<string> strvec = split(ID_setup.at(0),':');
        DeleteSpace(ID_setup.at(1));
        setup.push_back(ID_setup.at(1));
        axes.push_back(ID_setup.at(2));
        DeleteSpace(strvec.at(1));
        DeleteSpace(strvec.at(2));
        Dim.push_back(atoi(strvec.at(2).c_str()));
        data.push_back(strvec.at(1));
   }
    file.seekg(0,ios_base::beg);
    return data;
}

int HFiles::ReadDim(int i){
  return Dim.at(i);
}

vector<string> HFiles::ReadSetup(int i){
    vector<string> data;
    vector<string> strvec = split(setup.at(i),':');
    for(int i=0; i<strvec.size(); i++){
      data.push_back(strvec.at(i));
    }
    return data;
}


vector<string> HFiles::ReadAxes(int i){
    vector<string> data;
    vector<string> strvec = split(axes.at(i),':');
    for(int i=0; i<strvec.size(); i++){
      data.push_back(strvec.at(i));
    }
    return data;
}

vector<string> HFiles::ReadCSV(){

    vector<string> data;
    while(getline(file,buf)){
        DeleteSpace(buf);
        if(buf[0]=='#')
            continue;
        if(buf.empty())
            continue;
        vector<string> strvec = split(buf,',');
        for(int i=0; i<strvec.size(); i++){
            data.push_back(strvec.at(i));
        }
    }
    file.seekg(0,ios_base::beg);
    return data;
}


vector<string> HFiles::ReadData(int NofData){

    vector<string> data;
    int j=0;
    while(getline(file,buf)&&j<NofData){
        if(buf[0]=='#')
            continue;
        if(buf.empty())
            continue;
        DeleteSpace(buf);
        data.push_back(buf);
        j++;
    }
    file.seekg(0,ios_base::beg);
    return data;
}

