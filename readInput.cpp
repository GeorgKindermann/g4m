#ifndef READINPUT_CPP
#define READINPUT_CPP
#include <algorithm>
#include <map>
#include "misc.h"

using namespace std;

pair<double, string> getNumber(string str);

vector< pair<double, string> > header;

//******************************************************************************
// reading data with resolution 0,5x0,5
//******************************************************************************
void readInput(string FileName,dataArray &data)
 {
//Opening a file  
  ifstream fp;
  FileName = homeDir + "in_" + FileName + ".txt";
  fp.open(FileName.c_str(), ios::in);
  if (!fp.is_open()) {
    cout << "Cannot read " << FileName << endl;
    system("pause");
    exit(0);
  }
  cout << "> Reading file... " << FileName << endl;
  data.resize(ResLongitude);
  for (int i=0; i<ResLongitude; i++) data[i].resize(ResLatitude);
  string line;
  getline(fp,line);
  stringstream ss(line);
  string buf;
  ss >> buf;                      // skip x
  ss >> buf;                      // skip y
  std::vector<int> years;
// Reading header line
  while (ss >> buf) 
    years.push_back(getNumber(buf).first);
  while (!fp.eof())
   {
    getline(fp,line);
    if(line[0] != '#' && line.size()>0)
     {
      stringstream ss(line);
      double x,y,val;
      ss >> x;
      ss >> y;
      int LatIndex = int((y-GridStepLat/2+90)/GridStepLat);
      int LonIndex = int((x-GridStepLon/2+180)/GridStepLon);
      int ElemNum = 0;
      while (ss >> val)
       {
        if (years[ElemNum] <= eyear)
          data[LonIndex][LatIndex].insert(years[ElemNum],val);
        else break;
        ElemNum++;
       }
     }
   }
  fp.close();
  cout << "Successfully read..." << endl;
// finished reading file
 }
 
//******************************************************************************
// reads detailed data for plots from input file
//******************************************************************************
void readInputDet(dataDetStruct &data)
 {
//Opening a file  
  ifstream fp;
  string FileName = homeDir + "DataDet.txt";
  fp.open(FileName.c_str(), ios::in);
  if (!fp.is_open()) {
    cout << "Cannot read " << FileName << endl;
    system("pause");
    exit(0);
  }
  cout << "> Reading the rest of input data..." << endl;
  string line;
  getline(fp,line);
  stringstream ss(line);
  string buf;
  ss >> buf; ss >> buf;                      // skip x,y
//  ss >> buf;                                 // skip classes
  //int nnn=0;
  while (ss >> buf)
   {
    header.push_back(getNumber(buf));
    transform(header.back().second.begin(),header.back().second.end(),
              header.back().second.begin(), (int(*)(int)) toupper);
   }
   
  while (!fp.eof())
   {
    getline(fp,line);
    if(line[0] != '#' && line.size()>0)
     {
      g4m::dataStruct d;
      stringstream ss(line);
      double val,x,y;
      ss >> x;
      ss >> y;
//      ss >> d.classes;
      d.x = int((x-GridStepLon/2+180)/GridStepLon);
      d.y = int((y-GridStepLat/2+90)/GridStepLat);
      int ElemNum = 0;
      while (ss >> val)
       {
        if (header[ElemNum].first <= eyear)
         {
          if (!strcmp(header[ElemNum].second.c_str(),"NPP"))
            d.NPP.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"AGRSUIT"))
            d.AGRSUIT.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"BIOMASS"))
            d.BIOMASS.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"FOREST"))
            d.FOREST.insert(header[ElemNum].first,val);
          else if (!strcmp(header[ElemNum].second.c_str(),"PROTECT"))
            d.PROTECT.insert(header[ElemNum].first,val);
          //else if (!strcmp(header[ElemNum].second.c_str(),<prop>))
          //  d.<prop>.insert(header[ElemNum].first,val);
          // ...
         }
        ElemNum++;
       }
      data.push_back(d);
     }
   }
  fp.close();
  cout << "Successfully read." << endl;
// finished reading file
 }

//******************************************************************************
// reading coefficients
//******************************************************************************
void readCoeff(g4m::coeffStruct &coeff)
 {
//Opening a file  
  ifstream fp;
  string FileName = homeDir + "coeff.txt";
  fp.open(FileName.c_str(), ios::in);
  if (!fp.is_open()) {
    cout << "Cannot read " << FileName << endl;
    system("pause");
    exit(0);
  }
  cout << "> Reading coefficients..." << endl;
  while (!fp.eof())
    {
     string line;
     getline(fp, line);
     if(line[0] != '#')                        //Jump over lines starting with #
       {
        transform(line.begin(),line.end(),line.begin(), (int(*)(int)) toupper);
        stringstream ss(line);
        string buf;
        double val;
        ss >> buf;
        pair<double,string> tmp=getNumber(buf);
        ss >> val;
        if (!strcmp(tmp.second.c_str(),"BYEAR"))
          coeff.bYear = int(val);
        else if (!strcmp(tmp.second.c_str(),"EYEAR"))
          coeff.eYear = int(val);
        else if (!strcmp(tmp.second.c_str(),"CELLSINTERACT"))
          coeff.cellsInteract = int(val);
        else if (!strcmp(tmp.second.c_str(),"INCLAFFOR"))
          coeff.inclAffor = int(val);
        else if (!strcmp(tmp.second.c_str(),"NOPAY"))
          coeff.noPay = int(val);
        else if (!strcmp(tmp.second.c_str(),"UBIOMASS"))
          coeff.uBiomass = int(val);
        else if (!strcmp(tmp.second.c_str(),"LITTER"))
          coeff.litter = int(val);
        else if (!strcmp(tmp.second.c_str(),"SOC"))
          coeff.SOC = int(val);
        else if (!strcmp(tmp.second.c_str(),"PRICELANDMINR"))
          coeff.PriceLandMinR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICELANDMAXR"))
          coeff.PriceLandMaxR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"FCUPTAKE"))
          coeff.FCuptake.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"FTIMBER"))
          coeff.FTimber.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"HARVLOOS"))
          coeff.HarvLoos.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICEC"))
          coeff.PriceC.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"FRACLONGPROD"))
          coeff.FracLongProd.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"DECRATEL"))
          coeff.decRateL.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"DECRATES"))
          coeff.decRateS.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"SLASHBURN"))
          coeff.SlashBurn.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"FREQAID"))
          coeff.FreqAid.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICECAID"))
          coeff.PriceCAid.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"MAXROTINTER"))
          coeff.MaxRotInter.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"MINROTINTER"))
          coeff.MinRotInter.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"BASELINE"))
          coeff.baseline.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICETIMBERMAXR"))
          coeff.PriceTimberMaxR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICETIMBERMINR"))
          coeff.PriceTimberMinR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PRICEINDEXE"))
          coeff.PriceIndexE.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"PLANTINGCOSTS"))
          coeff.PlantingCostsR.insert(tmp.first,val);
        else if (!strcmp(tmp.second.c_str(),"SPOPDENS"))
          coeff.sPopDens.insert(tmp.first,val);
       }
    }
  fp.close();
  cout << "Coefficients successfully read..." << endl;
// finished reading file
 }

//******************************************************************************
// splits string to pair <number,string>
//******************************************************************************
pair<double, string> getNumber(string str)
{ // Returns a pair of double, string;
  string text;
  string number;
  for(unsigned int i=0; i < str.size(); ++i) {
    if((str[i] >= '0' && str[i] <= '9') || str[i] == '.')
       number += str[i];
    else if((str[i] != '[') and (str[i] != ']'))
      text += str[i];
  }
  double x = 0;
  stringstream ss(number);
  ss >> x;
  return(pair<double, string>(x, text));
}

#endif
