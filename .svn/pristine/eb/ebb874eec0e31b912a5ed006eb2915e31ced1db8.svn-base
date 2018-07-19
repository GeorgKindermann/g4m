using namespace std;

//******************************************************************************
// types
//******************************************************************************
typedef vector< g4m::ipol<double,double> > cellCol;
typedef vector<cellCol> dataArray;
typedef vector<g4m::dataStruct> dataDetStruct;

//******************************************************************************
// containers of data
//******************************************************************************
dataArray data05x_BUILTUP;
dataArray data05x_CROP;
dataArray data05x_AREA;
dataArray data05x_PRICEINDEX;
dataArray data05x_POPDENS;
dataArray data05x_DISCRATE;
dataArray data05x_GDP;
dataArray data05x_COUNTRY;
//dataArray data05x_NOTUSABLE;
vector<g4m::dataStruct> dataDet;
g4m::coeffStruct coeff;
//******************************************************************************
// constants and variables
//******************************************************************************
#ifdef unix
string homeDir = "./data/";
#else
string homeDir = "data\\";
#endif
int ResLatitude, ResLongitude;    // resolutions of model
int eyear, byear;
const double GridStepLat = 0.5;   // step by latitude
const double GridStepLon = 0.5;   // step by longitude
const int nYears = 110;
//******************************************************************************
// functions
//******************************************************************************
void readInput(string, dataArray &);
void readInputDet(dataDetStruct &);
double procPlots(g4m::dataStruct &,double,int);
double calcPlots(double [],int);
