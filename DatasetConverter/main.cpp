#include <iostream>
#include "progress_bar.hpp"
#include "fstream"
#include <sstream>
#include <vector>
#include <iomanip>
#include <limits>

using dbl=std::numeric_limits<double>;
//darknet order
enum DL{id,x,y,w,h};

struct Point{
    double x;
    double y;
};

struct Darknet{
    Darknet()=default;

    void setInput(std::vector<std::string>& stringInput){
        xc=::atof(stringInput[DL::x].c_str());
        yc=::atof(stringInput[DL::y].c_str());
        w=::atof(stringInput[DL::w].c_str());
        h=::atof(stringInput[DL::h].c_str());
        id=::atoi(stringInput[DL::id].c_str());
    }

    void Darknet2Pytorch(){
        UL.x=xc-w/2;
        UL.y=yc-h/2;

        LR.x=xc+w/2;
        LR.y=yc+h/2;
    }

    double xc;
    double yc;
    double w;
    double h;
    int id;

    Point UL; //upper left
    Point LR; //lower right

};

void readFileName(std::string& filename,std::vector<std::string>& outputLines);

Darknet parseLines(std::string& inputLine, Darknet &darknet, bool &isFormatOK);

void readTextFile(std::string& eachImagepath, std::vector<std::string> &lines);

void writeLines(std::string& imagePath,std::vector<std::string>& labels,std::string& outputPath){
    static Darknet darknet;
    std::fstream stream;
    stream.open(outputPath,std::ios::out | std::ios::app);
    stream<<imagePath;
    for(std::string& label:labels){
        bool isFormatOK;
        Darknet darknet=parseLines(label,darknet,isFormatOK);
        if(stream.is_open() && isFormatOK){
            stream<<" "<<darknet.UL.x<<","<<darknet.UL.y<<","<<
                    darknet.LR.x<<","<<darknet.LR.y<<","<<
                    darknet.id;
        }
    }
    stream<<" \n";
}

int main(int argc,char* argv[]) {
    if(argc!=3){
        std::cout<<"please use this file like the below"<<std::endl<<std::endl;
        std::cout<<"./ConvertDarknetDataset [inputFileName] [outputFile]"<<std::endl<<std::endl;
        std::abort();
    }
    ProgressBar *bar ;
    std::string filename=argv[1];//"/home/isv/Documents/datasets/train.txt";
    std::string outputPath=argv[2];//"/home/isv/Documents/datasets/pytorchTrain.txt";
    std::vector<std::string> readLines,lineofTexts;
    readFileName(filename,readLines);
    bar = new ProgressBar(readLines.size(), "processing files");
    int processed=0;
    for(std::string& imagePathStr:readLines){
        readTextFile(imagePathStr,lineofTexts);
        writeLines(imagePathStr,lineofTexts,outputPath);
        bar->Progressed(processed);
        processed++;
    }

    std::cout<<"files done"<<std::endl;
}

void readTextFile(std::string& eachImagepath,std::vector<std::string>& lines){
    lines.resize(0);
    static std::string textPath;
    textPath="";

    std::istringstream f(eachImagepath);
    if (getline(f, textPath, '.')) {
        textPath=textPath+".txt";
    }

    std::ifstream inFile;
    inFile.open(textPath,std::ios::in);
    if(!inFile){
        std::cout<<textPath<<" can't read this file"<<std::endl;
        return;
    }

    static std::string line;
    line="";
    while(getline(inFile,line)){
        lines.push_back(line);
    }
}
void readFileName(std::string& filename,std::vector<std::string>& outputLines){
    outputLines.clear();
    outputLines.reserve(10000);
    std::ifstream inFile;
    inFile.open(filename,std::ios::in);
    if(!inFile){
        std::cout<<"Unable to open File"<<std::endl;
        std::abort();
    }

    std::string line;
    while (getline(inFile,line)) {
        outputLines.push_back(line);
    }
}

Darknet parseLines(std::string& inputLine,Darknet& darknet,bool & isFormatOK){
    isFormatOK=true;
    std::vector<std::string> output;
    std::istringstream f(inputLine);
    while (getline(f, inputLine, ' ')) {
        output.push_back(inputLine);
    }
    Darknet darknetPoint;
    if(output.size()!=5){
        isFormatOK=false;
        return darknetPoint;
    }
    darknetPoint.setInput(output);
    darknetPoint.Darknet2Pytorch();
    return darknetPoint;
}
