#ifndef WITCH_RUNMANAGER_HH
#define WITCH_RUNMANAGER_HH

#include "Wisard_Global.hh"
#include "G4RunManager.hh"
#include "Wisard_Sensor.hh"
#include "G4AnalysisManager.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TTreeReader.h"
#include <TDirectory.h>

#include <unordered_map>
#include <utility>
#include <vector>

class Wisard_RunManager : public G4RunManager
{

protected:
  Wisard_Sensor *wisard_sensor_PlasticScintillator;
  Wisard_Sensor *wisard_sensor_CatcherMylar_central;
  Wisard_Sensor *wisard_sensor_CatcherAl1_central;
  Wisard_Sensor *wisard_sensor_CatcherAl2_central;
  Wisard_Sensor *wisard_sensor_CatcherMylar_side;
  Wisard_Sensor *wisard_sensor_CatcherAl1_side;
  Wisard_Sensor *wisard_sensor_CatcherAl2_side;

  /// TREE VARIABLES ////
  vector<G4int> Particle_PDG;
  vector<G4double> x;
  vector<G4double> y; 
  vector<G4double> z;
  vector<G4double> px; 
  vector<G4double> py; 
  vector<G4double> pz;
  vector<G4double> Kinetic_Energy;

  G4double PlasticScintillator_Deposit_Energy;
  vector<G4double>  PlasticScintillator_Proton_Hit_Position, PlasticScintillator_Positron_Hit_Position;
  G4double PlasticScintillator_Proton_Hit_Angle, PlasticScintillator_Positron_Hit_Angle;

  vector<G4double> Silicon_Detector_Deposit_Energy, Silicon_Detector_DL_Deposit_Energy;
  vector<vector<G4double>> Silicon_Detector_Proton_Hit_Position, Silicon_Detector_Positron_Hit_Position;
  vector<G4double> Silicon_Detector_Proton_Hit_Angle, Silicon_Detector_Positron_Hit_Angle;
  vector<G4int> Silicon_Detector_Code;
  
  G4double  Catcher_Central_Deposit_Energy, Catcher_Side_Deposit_Energy;
  ///////////////////////

  G4int count = 0;

  ifstream input_txt;
  string input_name;

  ifstream inputSRIM;
  string input_nameSRIM;

  G4double threshold;
  G4String filename;

public:
  // constructor and destructor
  Wisard_RunManager();
  ~Wisard_RunManager();

  static const int nb_det = 40;

  TFile *f;

  TTree *Tree;

  TH1D *silicon_coinc[nb_det];
  TH1D *silicon_nocoinc[nb_det];
  TH1D *silicon_single[nb_det];
  TH1D *plastic_coinc;

  std::string Detector_Name[nb_det] = {
      "1Up_Strip_1", "1Up_Strip_2", "1Up_Strip_3", "1Up_Strip_4", "1Up_Strip_5",
      "2Up_Strip_1", "2Up_Strip_2", "2Up_Strip_3", "2Up_Strip_4", "2Up_Strip_5",
      "3Up_Strip_1", "3Up_Strip_2", "3Up_Strip_3", "3Up_Strip_4", "3Up_Strip_5",
      "4Up_Strip_1", "4Up_Strip_2", "4Up_Strip_3", "4Up_Strip_4", "4Up_Strip_5",
      "1Down_Strip_1", "1Down_Strip_2", "1Down_Strip_3", "1Down_Strip_4", "1Down_Strip_5",
      "2Down_Strip_1", "2Down_Strip_2", "2Down_Strip_3", "2Down_Strip_4", "2Down_Strip_5",
      "3Down_Strip_1", "3Down_Strip_2", "3Down_Strip_3", "3Down_Strip_4", "3Down_Strip_5",
      "4Down_Strip_1", "4Down_Strip_2", "4Down_Strip_3", "4Down_Strip_4", "4Down_Strip_5"};

  int Detector_Code[nb_det] = {
      11, 12, 13, 14, 15,
      21, 22, 23, 24, 25, 
      31, 32, 33, 34, 35,
      41, 42, 43, 44, 45,
      -11, -12, -13, -14, -15,
      -21, -22, -23, -24, -25,
      -31, -32, -33, -34, -35,
      -41, -42, -43, -44, -45};
      
  std::unordered_map<int, std::pair<Wisard_Sensor *, Wisard_Sensor *>> dic_detector;

  Wisard_Sensor *GetWisardSensor_PlasticScintillator();
  Wisard_Sensor *GetWisardSensor_Detector(string name);
  Wisard_Sensor *GetWisardSensor_CatcherMylar_central();
  Wisard_Sensor *GetWisardSensor_CatcherAl1_central();
  Wisard_Sensor *GetWisardSensor_CatcherAl2_central();
  Wisard_Sensor *GetWisardSensor_CatcherMylar_side();
  Wisard_Sensor *GetWisardSensor_CatcherAl1_side();
  Wisard_Sensor *GetWisardSensor_CatcherAl2_side();

  //----------------------------------------------------------
  // Commands definitions
  ////--------------------------------------------------
  //// U.I. commands creation function
  // void  DefineSimulationCommands ( );     // inline
  //----------------------------------------------------------
  // Functions for input file
  ////--------------------------------------------------
  //// added input file functions
  ////                  declaration
  int OpenInput(const string &fname);
  void CloseInput();                  // inline
  ifstream &GetInput_TXT();               // inline
  const string &GetInputName() const; // inline

  int OpenInputSRIM(const string &fname);
  void CloseInputSRIM();    // inline
  ifstream &GetInputSRIM(); // inline

  void SetThreshold(G4double th);
  G4double GetThreshold();

  void SetOutputFilename(G4String fn);
  G4String GetOutputFilename();

  //----------------------------------------------------------
  // Functions for events processing and output histogram

  // redefine the function from base class G4RunManager
  void AnalyzeEvent(G4Event *event);
};

//----------------------------------------------------------------------
// inline functions for the class

inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_Detector(string name)
{

  if (name.substr(name.length() - 2) == "dl")
  {
    if (dic_detector.find( stoi( name.substr(0, name.length() - 3)) ) != dic_detector.end())
    {
      return (dic_detector[ stoi( name.substr(0, name.length() - 3))].second);
    }
    else
    {
      G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }

  else
  {
    if (dic_detector.find( stoi( name)) != dic_detector.end())
    {
      return (dic_detector[ stoi( name)].first);
    }
    else
    {
      G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }
}

inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_PlasticScintillator() { return (wisard_sensor_PlasticScintillator); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherMylar_central() { return (wisard_sensor_CatcherMylar_central); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl1_central() { return (wisard_sensor_CatcherAl1_central); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl2_central() { return (wisard_sensor_CatcherAl2_central); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherMylar_side() { return (wisard_sensor_CatcherMylar_side); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl1_side() { return (wisard_sensor_CatcherAl1_side); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl2_side() { return (wisard_sensor_CatcherAl2_side); }

// Close the input file
inline void Wisard_RunManager::CloseInput()
{
  input_txt.close();
  input_name = "";
}

// Get the input file stream information
inline ifstream &Wisard_RunManager::GetInput_TXT()
{
  return (input_txt);
}

// Get the input file name
inline const string &Wisard_RunManager::GetInputName() const
{
  return (input_name);
}

// SRIM
inline void Wisard_RunManager::CloseInputSRIM()
{
  inputSRIM.close();
  input_nameSRIM = "";
}
inline ifstream &Wisard_RunManager::GetInputSRIM()
{
  return (inputSRIM);
}

// THRESHOOLD
inline void Wisard_RunManager::SetThreshold(G4double th) { threshold = th; }
inline G4double Wisard_RunManager::GetThreshold() { return (threshold); }

// OUPUT FILE
inline void Wisard_RunManager::SetOutputFilename(G4String fn) { filename = fn; }
inline G4String Wisard_RunManager::GetOutputFilename() { return filename; }
#endif
