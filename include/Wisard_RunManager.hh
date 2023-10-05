#ifndef WITCH_RUNMANAGER_HH
#define WITCH_RUNMANAGER_HH

#include "../Wisard_Global.hh"
#include "G4RunManager.hh"
#include "Wisard_Sensor.hh"
#include "G4AnalysisManager.hh"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include <TDirectory.h>

#include <unordered_map>
#include <utility>

//----------------------------------------------------------------------
// This class defines the simulation core (from the base class G4RunManager)
// including information to get the required information
class Wisard_RunManager : public G4RunManager
{

  //------------------------------------------------------------
  // internal variables definition
protected:
  Wisard_Sensor *wisard_sensor_PlasticScintillator;
  Wisard_Sensor *wisard_sensor_CatcherMylar;
  Wisard_Sensor *wisard_sensor_CatcherAl1;
  Wisard_Sensor *wisard_sensor_CatcherAl2;

  G4double x, y, z;
  G4double Catcher_EnergyDeposit;
  G4double PlasticScintillator_EnergyDeposit;
  std::string Silicon_Name;
  G4double Silicon_EnergyDeposit;
  G4double Silicon_DL_EnergyDeposit;

  G4int Event_Number;
  int count = 0;

  G4double Initial_Proton_Momentum_x;
  G4double Initial_Proton_Momentum_y;
  G4double Initial_Proton_Momentum_z;
  G4double Initial_Proton_Energy;
  G4double Catcher_Proton_EnergyDeposit;
  G4double PlasticScintillator_Proton_EnergyDeposit;
  G4double Silicon_Proton_EnergyDeposit;
  G4double Silicon_DL_Proton_EnergyDeposit;
  G4double Silicon_Proton_HitAngle;
  std::string Silicon_Name_Proton;
  G4double Silicon_Proton_HitPosition_x;
  G4double Silicon_Proton_HitPosition_y;
  G4double Silicon_Proton_HitPosition_z;

  G4double Initial_Alpha_Momentum_x;
  G4double Initial_Alpha_Momentum_y;
  G4double Initial_Alpha_Momentum_z;
  G4double Initial_Alpha_Energy;
  G4double Catcher_Alpha_EnergyDeposit;
  G4double PlasticScintillator_Alpha_EnergyDeposit;
  G4double Silicon_Alpha_EnergyDeposit;
  G4double Silicon_DL_Alpha_EnergyDeposit;
  G4double Silicon_Alpha_HitAngle;
  std::string Silicon_Name_Alpha;
  G4double Silicon_Alpha_HitPosition_x;
  G4double Silicon_Alpha_HitPosition_y;
  G4double Silicon_Alpha_HitPosition_z;

  G4double Initial_Positron_Momentum_x;
  G4double Initial_Positron_Momentum_y;
  G4double Initial_Positron_Momentum_z;
  G4double Initial_Positron_Energy;
  G4double Catcher_Positron_EnergyDeposit;
  G4double PlasticScintillator_Positron_EnergyDeposit;
  G4double PlasticScintillator_Positron_HitAngle;
  G4double PlasticScintillator_Positron_HitPosition_x;
  G4double PlasticScintillator_Positron_HitPosition_y;
  G4double PlasticScintillator_Positron_HitPosition_z;

  G4double Initial_Electron_Momentum_x;
  G4double Initial_Electron_Momentum_y;
  G4double Initial_Electron_Momentum_z;
  G4double Initial_Electron_Energy;
  G4double Catcher_Electron_EnergyDeposit;
  G4double PlasticScintillator_Electron_EnergyDeposit;
  G4double PlasticScintillator_Electron_HitAngle;
  G4double PlasticScintillator_Electron_HitPosition_x;
  G4double PlasticScintillator_Electron_HitPosition_y;
  G4double PlasticScintillator_Electron_HitPosition_z;

  G4double Initial_Gamma_Momentum_x;
  G4double Initial_Gamma_Momentum_y;
  G4double Initial_Gamma_Momentum_z;
  G4double Initial_Gamma_Energy;
  G4double Catcher_Gamma_EnergyDeposit;
  G4double Silicon_Gamma_EnergyDeposit;
  G4double Silicon_DL_Gamma_EnergyDeposit;
  G4double Silicon_Gamma_HitAngle;
  std::string Silicon_Name_Gamma;
  G4double Silicon_Gamma_HitPosition_x;
  G4double Silicon_Gamma_HitPosition_y;
  G4double Silicon_Gamma_HitPosition_z;
  G4double PlasticScintillator_Gamma_EnergyDeposit;
  G4double PlasticScintillator_Gamma_HitAngle;
  G4double PlasticScintillator_Gamma_HitPosition_x;
  G4double PlasticScintillator_Gamma_HitPosition_y;
  G4double PlasticScintillator_Gamma_HitPosition_z;

  G4double PlasticScintillator_Positron_Ekin;
  double e_PlasticScintillator;

  ifstream input;
  string input_name;

  ifstream inputSRIM;
  string input_nameSRIM;

  G4double threshoold;
  G4String filename;
  G4double resolution_sipms;
  G4double resolution_sidet;

  //------------------------------------------------------------
  // class functions definition
public:
  // constructor and destructor
  Wisard_RunManager();
  ~Wisard_RunManager();

  static const int nb_det = 40;

  TFile *f;

  TTree *Tree_Common;
  TTree *Tree_Proton;
  TTree *Tree_Gamma;
  TTree *Tree_Alpha;
  TTree *Tree_Electron;
  TTree *Tree_Positron;

  TH1D *histos_coinc[nb_det];
  TH1D *histos_nocoinc[nb_det];


  std::string Detector_Name[nb_det] = {
      "1Up_Strip_1", "1Up_Strip_2", "1Up_Strip_3", "1Up_Strip_4", "1Up_Strip_5",
      "2Up_Strip_1", "2Up_Strip_2", "2Up_Strip_3", "2Up_Strip_4", "2Up_Strip_5",
      "3Up_Strip_1", "3Up_Strip_2", "3Up_Strip_3", "3Up_Strip_4", "3Up_Strip_5",
      "4Up_Strip_1", "4Up_Strip_2", "4Up_Strip_3", "4Up_Strip_4", "4Up_Strip_5",
      "1Down_Strip_1", "1Down_Strip_2", "1Down_Strip_3", "1Down_Strip_4", "1Down_Strip_5",
      "2Down_Strip_1", "2Down_Strip_2", "2Down_Strip_3", "2Down_Strip_4", "2Down_Strip_5",
      "3Down_Strip_1", "3Down_Strip_2", "3Down_Strip_3", "3Down_Strip_4", "3Down_Strip_5",
      "4Down_Strip_1", "4Down_Strip_2", "4Down_Strip_3", "4Down_Strip_4", "4Down_Strip_5"};

  std::unordered_map<std::string, std::pair<Wisard_Sensor *, Wisard_Sensor *>> dic_detector;

  Wisard_Sensor *GetWisardSensor_PlasticScintillator();
  Wisard_Sensor *GetWisardSensor_Detector(string name);
  Wisard_Sensor *GetWisardSensor_CatcherMylar();
  Wisard_Sensor *GetWisardSensor_CatcherAl1();
  Wisard_Sensor *GetWisardSensor_CatcherAl2();

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
  ifstream &GetInput();               // inline
  const string &GetInputName() const; // inline

  int OpenInputSRIM(const string &fname);
  void CloseInputSRIM();    // inline
  ifstream &GetInputSRIM(); // inline

  void SetThreshoold(G4double th);
  G4double GetThreshoold();

  void SetOutputFilename(G4String fn);
  G4String GetOutputFilename();

  void SetResolutionSIPMS(G4double value);
  void SetResolutionSIDET(G4double value);

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
    if (dic_detector.find(name.substr(0, name.length() - 3)) != dic_detector.end())
    {
      return (dic_detector[name.substr(0, name.length() - 3)].second);
    }
    else
    {
      G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }

  else
  {
    if (dic_detector.find(name) != dic_detector.end())
    {
      return (dic_detector[name].first);
    }
    else
    {
      G4Exception("GetWisard_Sensor_GetDetector", ("Detector Name not found : " + name).c_str(), JustWarning, "");
      return nullptr;
    }
  }
}

inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_PlasticScintillator() { return (wisard_sensor_PlasticScintillator); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherMylar() { return (wisard_sensor_CatcherMylar); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl1() { return (wisard_sensor_CatcherAl1); }
inline Wisard_Sensor *Wisard_RunManager::GetWisardSensor_CatcherAl2() { return (wisard_sensor_CatcherAl2); }

// Close the input file
inline void Wisard_RunManager::CloseInput()
{
  input.close();
  input_name = "";
}

// Get the input file stream information
inline ifstream &Wisard_RunManager::GetInput()
{
  return (input);
}

// Get the input file name
inline const string &Wisard_RunManager::GetInputName() const
{
  return (input_name);
}

//SRIM
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
inline void Wisard_RunManager::SetThreshoold(G4double th){threshoold = th;}
inline G4double Wisard_RunManager::GetThreshoold(){return (threshoold);}

// OUPUT FILE
inline void Wisard_RunManager::SetOutputFilename(G4String fn){filename = fn;}
inline G4String Wisard_RunManager::GetOutputFilename(){return filename;}

// SiPMs RESOLUTION
inline void Wisard_RunManager::SetResolutionSIPMS(G4double res){resolution_sipms = res;}

// SiDET RESOLUTION
inline void Wisard_RunManager::SetResolutionSIDET(G4double res){resolution_sidet = res;}
#endif
