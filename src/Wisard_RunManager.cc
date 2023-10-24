#include "Wisard_RunManager.hh"
#include "CLHEP/Random/RandGauss.h"
#include "G4AnalysisManager.hh"

#include <iostream>

#include "TBranch.h"
#include "TH1D.h"
#include "TObjString.h"

#include "G4VTrajectoryPoint.hh"
#include "G4VTrajectory.hh"

#include "Wisard_Sensor.hh"

#include "G4RootAnalysisManager.hh"

#include "G4UImanager.hh"

#include <numeric>

//----------------------------------------------------------------------

// constructor
Wisard_RunManager::Wisard_RunManager()
{
  cout << "Constructor Wisard_RunManager" << endl;

  ////////////// Construct all sensors //////////////////////////////
  cout << "Constructor Wisard_Sensor" << endl;
  wisard_sensor_PlasticScintillator = new Wisard_Sensor;
  for (int i = 0; i < nb_det; i++)
  {
    dic_detector[Detector_Name[i]] = std::make_pair(new Wisard_Sensor, new Wisard_Sensor);
  }
  wisard_sensor_CatcherMylar = new Wisard_Sensor;
  wisard_sensor_CatcherAl1 = new Wisard_Sensor;
  wisard_sensor_CatcherAl2 = new Wisard_Sensor;
  ///////////////////////////////////////////////////////////////////

  ////////////// Construct strips coinc/anticoinc HISTOGRAMS ////////
  for (int i = 0; i < nb_det; i++)
  {
    histos_coinc[i] = new TH1D((Detector_Name[i] + "_coinc").c_str(), (Detector_Name[i] + "_coinc").c_str(), 100000, 0.0, 10000.0);
    histos_nocoinc[i] = new TH1D((Detector_Name[i] + "_nocoinc").c_str(), (Detector_Name[i] + "_nocoinc").c_str(), 100000, 0.0, 10000.0);
  }
  ///////////////////////////////////////////////////////////////////
}

// destructor
Wisard_RunManager::~Wisard_RunManager()
{
  cout << "Destructor Wisard_RunManager" << endl;

  f->Close();
  delete f;

  cout << "Destructor Wisard_Sensor" << endl;
  delete wisard_sensor_PlasticScintillator;
  for (int i = 0; i < nb_det; i++)
  {
    delete dic_detector[Detector_Name[i]].first;
    delete dic_detector[Detector_Name[i]].second;
  }
  delete wisard_sensor_CatcherMylar;
  delete wisard_sensor_CatcherAl1;
  delete wisard_sensor_CatcherAl2;

  CloseInput();
}

//----------------------------------------------------------------------
void Wisard_RunManager::AnalyzeEvent(G4Event *event)
{
  if (event->GetEventID() == 0)
  {
    f = new TFile(GetOutputFilename(), "recreate");
    ////////////// Construct Log /////////////////////////////////////
    TDirectory *dir = f->mkdir("Log");
    dir->cd();
    G4UImanager *uiManager = G4UImanager::GetUIpointer();
    for (int i = 0; i < uiManager->GetNumberOfHistory(); i++)
    {
      string command = uiManager->GetPreviousCommand(i).substr(1);
      if (command.find("run") != 0 && command.find("event") != 0 && command.find("tracking") != 0 && command.find("input") != 0 && command.find("output") != 0)
      {
        char const *num_char = command.c_str();
        dir->WriteObject(&command, num_char);
      }
      f->cd();
    }
    //////////////////////////////////////////////////////////////////

    ////////////// Construct the TREE ////////////////////////////////
    Tree = new TTree("Tree", "Informations");
    Tree->Branch("Event_Number", &Event_Number, "Event_Number/I");
    Tree->Branch("Particle_Name", &Name);
    Tree->Branch("x", &x, "x/D");
    Tree->Branch("y", &y, "y/D");
    Tree->Branch("z", &z, "z/D");
    Tree->Branch("px", &px, "px/D");
    Tree->Branch("py", &py, "py/D");
    Tree->Branch("pz", &pz, "pz/D");
    Tree->Branch("Initial_Kinetic_Energy", &Initial_Kinetic_Energy, "Initial_Kinetic_Energy/D");
    Tree->Branch("Catcher_Deposit_Energy", &Catcher_Deposit_Energy, "Catcher_Deposit_Energy/D");
    Tree->Branch("PlasticScintillator_Deposit_Energy", &PlasticScintillator_Deposit_Energy, "PlasticScintillator_Deposit_Energy/D");
    Tree->Branch("PlasticScintillator_Hit_Position_x", &PlasticScintillator_Hit_Position_x, "PlasticScintillator_Hit_Position_x/D");
    Tree->Branch("PlasticScintillator_Hit_Position_y", &PlasticScintillator_Hit_Position_y, "PlasticScintillator_Hit_Position_y/D");
    Tree->Branch("PlasticScintillator_Hit_Position_z", &PlasticScintillator_Hit_Position_z, "PlasticScintillator_Hit_Position_z/D");
    Tree->Branch("PlasticScintillator_Hit_Angle", &PlasticScintillator_Hit_Angle, "PlasticScintillator_Hit_Angle/D");
    Tree->Branch("Silicon_Detector_Deposit_Energy", &Silicon_Detector_Deposit_Energy);
    Tree->Branch("Silicon_Detector_Hit_Position_x", &Silicon_Detector_Hit_Position_x);
    Tree->Branch("Silicon_Detector_Hit_Position_y", &Silicon_Detector_Hit_Position_y);
    Tree->Branch("Silicon_Detector_Hit_Position_z", &Silicon_Detector_Hit_Position_z);
    Tree->Branch("Silicon_Detector_Hit_Angle", &Silicon_Detector_Hit_Angle);
    Tree->Branch("Silicon_Detector_Name", &Silicon_Detector_Name);
    Tree->Branch("Silicon_Detector_DL_Deposit_Energy", &Silicon_Detector_DL_Deposit_Energy);
    ///////////////////////////////////////////////////////////////////
  }
  // call the base class function (whatever it is supposed to do)
  G4RunManager::AnalyzeEvent(event);

  G4PrimaryVertex *PrimaryVertex = event->GetPrimaryVertex();
  if (PrimaryVertex != 0)
  {
    count++;
    for (int part = 1; part <= event->GetNumberOfPrimaryVertex(); part++)
    {
      
      G4PrimaryParticle *Primary = event->GetPrimaryVertex(part - 1)->GetPrimary();
      G4ThreeVector Momentum = Primary->GetMomentumDirection();

      Event_Number = event->GetEventID();
      Name = Primary->GetG4code()->GetParticleName();
      x = PrimaryVertex->GetX0() / um;
      y = PrimaryVertex->GetY0() / um;
      z = PrimaryVertex->GetZ0() / nm;
      px = Momentum.x();
      py = Momentum.y();
      pz = Momentum.z();
      Initial_Kinetic_Energy = Primary->GetKineticEnergy() / keV;
      Catcher_Deposit_Energy = wisard_sensor_CatcherMylar->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl1->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl2->GetDictionnary()[part].DepositEnergy;

      PrimaryInfo PlasticScintillator = wisard_sensor_PlasticScintillator->GetDictionnary()[part];
      PlasticScintillator_Deposit_Energy = PlasticScintillator.DepositEnergy;
      PlasticScintillator_Hit_Position_x = PlasticScintillator.HitPosition.x();
      PlasticScintillator_Hit_Position_y = PlasticScintillator.HitPosition.y();
      PlasticScintillator_Hit_Position_z = PlasticScintillator.HitPosition.z();
      PlasticScintillator_Hit_Angle = PlasticScintillator.HitAngle;

      for (int i = 0; i < nb_det; i++)
      {
        PrimaryInfo dic_det = dic_detector[Detector_Name[i]].first->GetDictionnary()[part];
        PrimaryInfo dic_dl = dic_detector[Detector_Name[i]].second->GetDictionnary()[part];
        if (dic_detector[Detector_Name[i]].first->GetDictionnary()[part].DepositEnergy != 0)
        {
          Silicon_Detector_Name.push_back(Detector_Name[i]);
          Silicon_Detector_Deposit_Energy.push_back(max(dic_det.DepositEnergy+G4RandGauss::shoot(0.0, resolution_sidet/keV), 0.0));
          Silicon_Detector_Hit_Position_x.push_back(dic_det.HitPosition.x());
          Silicon_Detector_Hit_Position_y.push_back(dic_det.HitPosition.y());
          Silicon_Detector_Hit_Position_z.push_back(dic_det.HitPosition.z());
          Silicon_Detector_Hit_Angle.push_back(dic_det.HitAngle);
          Silicon_Detector_DL_Deposit_Energy.push_back(dic_dl.DepositEnergy);
        }
      }
      Tree->Fill();
    }
    
    
    ///// Reset all variables of detectors pour the next primary particle /////////////////
    x = 0;
    y = 0;
    z = 0;
    px = 0;
    py = 0;
    pz = 0;
    Initial_Kinetic_Energy = 0;
    Catcher_Deposit_Energy = 0;
    PlasticScintillator_Deposit_Energy = 0;
    PlasticScintillator_Hit_Position_x = 0;
    PlasticScintillator_Hit_Position_x = 0;
    PlasticScintillator_Hit_Position_x = 0;
    PlasticScintillator_Hit_Angle = 0;
    Silicon_Detector_Deposit_Energy.clear();
    Silicon_Detector_Hit_Position_x.clear();
    Silicon_Detector_Hit_Position_y.clear();
    Silicon_Detector_Hit_Position_z.clear();
    Silicon_Detector_Hit_Angle.clear();
    Silicon_Detector_Name.clear();
    Silicon_Detector_DL_Deposit_Energy.clear();
    ////////////////////////////////////////////////////////
  }

  // HISTOGRAMS ///#condition à revoir pour plus précis et générale
  int index_delayed = -1;
  int index_beta = -1;
  for (int part = 1; part <= event->GetNumberOfPrimaryVertex(); part++)
  {
    G4String part_name = event->GetPrimaryVertex(part - 1)->GetPrimary()->GetG4code()->GetParticleName();
    if (part_name == "e+" || part_name == "e-")
    {
      index_beta = part;
    }
    if (part_name == "alpha" || part_name == "proton")
    {
      if (index_beta != -1)
      {
        index_delayed = part;
        break;
      }
    }
  }

  if (index_beta != -1 && index_delayed != -1)
  {
    for (int i = 0; i < nb_det; i++)
    {
      if (dic_detector[Detector_Name[i]].first->GetDictionnary()[index_delayed].DepositEnergy != 0.)
      {
        if (wisard_sensor_PlasticScintillator->GetDictionnary()[index_beta].DepositEnergy >= GetThreshoold()/keV)
        {
          histos_coinc[i]->Fill(dic_detector[Detector_Name[i]].first->GetDictionnary()[index_delayed].DepositEnergy);
        }
        else
        {
          histos_nocoinc[i]->Fill(dic_detector[Detector_Name[i]].first->GetDictionnary()[index_delayed].DepositEnergy);
        }
      }
    }
  }

  int divi = 10000;

  ///// Writing all trees ///////////////////////////////////////////
  if (count % divi == 0)
  {
    Tree->AutoSave("FlushBaskets");

    for (int i = 0; i < nb_det; i++)
    {
      histos_coinc[i]->Write("", TObject::kOverwrite);
      histos_nocoinc[i]->Write("", TObject::kOverwrite);
    }
  }

  ///// Reset all dictionnaries of detectors ///////////////////////
  wisard_sensor_PlasticScintillator->ResetDictionnary();
  for (int i = 0; i < nb_det; i++)
  {
    dic_detector[Detector_Name[i]].first->ResetDictionnary();
    dic_detector[Detector_Name[i]].second->ResetDictionnary();
  }
  wisard_sensor_CatcherMylar->ResetDictionnary();
  wisard_sensor_CatcherAl1->ResetDictionnary();
  wisard_sensor_CatcherAl2->ResetDictionnary();
  ///////////////////////////////////////////////////////////////////
}

int Wisard_RunManager::OpenInput(const string &fname)
{
  int error = 0; // return value

  if (fname != "")
  {
    // close previous output... just in case
    CloseInput();

    // try to open the new file
    input.open(fname.c_str());

    if (input.fail())
    {
      error = 2;
      cerr << "<W> OpenInput : error opening file " << fname << endl;
    }
    else
    {
      input_name = fname;
      cout << "<I> Open input file: " << fname << endl;
    }
  }
  else
  {
    error = 1;
    cerr << "<W> OpenInput : empty file name" << endl;
  }

  return (error);
}

int Wisard_RunManager::OpenInputSRIM(const string &fname)
{
  int error = 0; // return value

  if (fname != "")
  {
    // close previous output... just in case
    CloseInputSRIM();

    // try to open the new file
    inputSRIM.open(fname.c_str());

    if (inputSRIM.fail())
    {
      error = 2;
      cerr << "<W> OpenInput : error opening file " << fname << endl;
    }
    else
    {
      input_nameSRIM = fname;
      cout << "<I> Open input file: " << fname << endl;
    }
  }
  else
  {
    error = 1;
    cerr << "<W> OpenInput : empty file name" << endl;
  }

  return (error);
}