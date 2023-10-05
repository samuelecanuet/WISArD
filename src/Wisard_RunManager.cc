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
    ////////////// Construct eLog /////////////////////////////////////
    TDirectory* dir = f->mkdir("eLog");
    dir->cd();
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    for (int i=0; i<uiManager->GetNumberOfHistory(); i++)
    { 
      string command = uiManager->GetPreviousCommand(i).substr(1);
      if (command.find("run") != 0 && command.find("event") != 0 && command.find("tracking") != 0 && command.find("input") != 0 && command.find("output") != 0) {
        char const *num_char = command.c_str();
        dir->WriteObject(&command, num_char);
    }
    f->cd();
    }


    ////////////// Construct all TREES ////////////////////////////////
    Tree_Common = new TTree("Tree_Common", "Common Information");
    Tree_Common->Branch("Event_Number", &Event_Number, "Event_Number/I");
    Tree_Common->Branch("x", &x, "x/D");
    Tree_Common->Branch("y", &y, "y/D");
    Tree_Common->Branch("z", &z, "z/D");
    Tree_Common->Branch("Catcher_EnergyDeposit", &Catcher_EnergyDeposit, "Catcher_EnergyDeposit/D");
    Tree_Common->Branch("PlasticScintillator_EnergyDeposit", &PlasticScintillator_EnergyDeposit, "PlasticScintillator_EnergyDeposit/D");
    Tree_Common->Branch("Silicon_EnergyDeposit", &Silicon_EnergyDeposit, "Catcher_EneSilicon_EnergyDepositrgyDeposit/D");
    Tree_Common->Branch("Silicon_DL_EnergyDeposit", &Silicon_DL_EnergyDeposit, "Silicon_DL_EnergyDeposit/D");
    Tree_Common->Branch("Silicon_Name_Proton", &Silicon_Name);

    Tree_Proton = new TTree("Tree_Proton", "Information Proton");
    Tree_Proton->Branch("Event_Number", &Event_Number, "Event_Number/I");
    Tree_Proton->Branch("Initial_Proton_Momentum_x", &Initial_Proton_Momentum_x, "Initial_Proton_Momentum_x/D");
    Tree_Proton->Branch("Initial_Proton_Momentum_y", &Initial_Proton_Momentum_y, "Initial_Proton_Momentum_y/D");
    Tree_Proton->Branch("Initial_Proton_Momentum_z", &Initial_Proton_Momentum_z, "Initial_Proton_Momentum_z/D");
    Tree_Proton->Branch("Initial_Proton_Energy", &Initial_Proton_Energy, "Initial_Proton_Energy/D");
    Tree_Proton->Branch("Catcher_Proton_EnergyDeposit", &Catcher_Proton_EnergyDeposit, "Catcher_Proton_EnergyDeposit/D");
    Tree_Proton->Branch("PlasticScintillator_Proton_EnergyDeposit", &PlasticScintillator_Proton_EnergyDeposit, "PlasticScintillator_Proton_EnergyDeposit/D");
    Tree_Proton->Branch("Silicon_Proton_EnergyDeposit", &Silicon_Proton_EnergyDeposit, "Silicon_Proton_EnergyDeposit/D");
    Tree_Proton->Branch("Silicon_DL_Proton_EnergyDeposit", &Silicon_DL_Proton_EnergyDeposit, "Silicon_DL_Proton_EnergyDeposit/D");
    Tree_Proton->Branch("Silicon_Proton_HitAngle", &Silicon_Proton_HitAngle, "Silicon_Proton_HitAngle/D");
    Tree_Proton->Branch("Silicon_Name_Proton", &Silicon_Name_Proton);
    Tree_Proton->Branch("Silicon_Proton_HitPosition_x", &Silicon_Proton_HitPosition_x, "Silicon_Proton_HitPosition_x/D");
    Tree_Proton->Branch("Silicon_Proton_HitPosition_y", &Silicon_Proton_HitPosition_y, "Silicon_Proton_HitPosition_y/D");
    Tree_Proton->Branch("Silicon_Proton_HitPosition_z", &Silicon_Proton_HitPosition_z, "Silicon_Proton_HitPosition_z/D");

    Tree_Alpha = new TTree("Tree_Alpha", "Information Alpha");
    Tree_Alpha->Branch("Event_Number", &Event_Number, "Event_Number/I");
    Tree_Alpha->Branch("Initial_Alpha_Momentum_x", &Initial_Alpha_Momentum_x, "Initial_Alpha_Momentum_x/D");
    Tree_Alpha->Branch("Initial_Alpha_Momentum_y", &Initial_Alpha_Momentum_y, "Initial_Alpha_Momentum_y/D");
    Tree_Alpha->Branch("Initial_Alpha_Momentum_z", &Initial_Alpha_Momentum_z, "Initial_Alpha_Momentum_z/D");
    Tree_Alpha->Branch("Initial_Alpha_Energy", &Initial_Alpha_Energy, "Initial_Alpha_Energy/D");
    Tree_Alpha->Branch("Catcher_Alpha_EnergyDeposit", &Catcher_Alpha_EnergyDeposit, "Catcher_Alpha_EnergyDeposit/D");
    Tree_Alpha->Branch("PlasticScintillator_Alpha_EnergyDeposit", &PlasticScintillator_Alpha_EnergyDeposit, "PlasticScintillator_Alpha_EnergyDeposit/D");
    Tree_Alpha->Branch("Silicon_Alpha_EnergyDeposit", &Silicon_Alpha_EnergyDeposit, "Silicon_Alpha_EnergyDeposit/D");
    Tree_Alpha->Branch("Silicon_DL_Alpha_EnergyDeposit", &Silicon_DL_Alpha_EnergyDeposit, "Silicon_DL_Alpha_EnergyDeposit/D");
    Tree_Alpha->Branch("Silicon_Alpha_HitAngle", &Silicon_Alpha_HitAngle, "Silicon_Alpha_HitAngle/D");
    Tree_Alpha->Branch("Silicon_Name_Alpha", &Silicon_Name_Alpha);
    Tree_Alpha->Branch("Silicon_Alpha_HitPosition_x", &Silicon_Alpha_HitPosition_x, "Silicon_Alpha_HitPosition_x/D");
    Tree_Alpha->Branch("Silicon_Alpha_HitPosition_y", &Silicon_Alpha_HitPosition_y, "Silicon_Alpha_HitPosition_y/D");
    Tree_Alpha->Branch("Silicon_Alpha_HitPosition_z", &Silicon_Alpha_HitPosition_z, "Silicon_Alpha_HitPosition_z/D");

    Tree_Positron = new TTree("Tree_Positron", "Information Positron");
    Tree_Positron->Branch("Event_Number", &Event_Number, "Event_Number/I");
    Tree_Positron->Branch("Initial_Positron_Momentum_x", &Initial_Positron_Momentum_x, "Initial_Positron_Momentum_x/D");
    Tree_Positron->Branch("Initial_Positron_Momentum_y", &Initial_Positron_Momentum_y, "Initial_Positron_Momentum_y/D");
    Tree_Positron->Branch("Initial_Positron_Momentum_z", &Initial_Positron_Momentum_z, "Initial_Positron_Momentum_z/D");
    Tree_Positron->Branch("Initial_Positron_Energy", &Initial_Positron_Energy, "Initial_Positron_Energy/D");
    Tree_Positron->Branch("Catcher_Positron_EnergyDeposit", &Catcher_Positron_EnergyDeposit, "Catcher_Positron_EnergyDeposit/D");
    Tree_Positron->Branch("PlasticScintillator_Positron_Ekin", &PlasticScintillator_Positron_Ekin, "PlasticScintillator_Positron_Ekin/D");
    Tree_Positron->Branch("PlasticScintillator_Positron_EnergyDeposit", &PlasticScintillator_Positron_EnergyDeposit, "PlasticScintillator_Positron_EnergyDeposit/D");
    Tree_Positron->Branch("PlasticScintillator_Positron_HitAngle", &PlasticScintillator_Positron_HitAngle, "PlasticScintillator_Positron_HitAngle/D");
    Tree_Positron->Branch("PlasticScintillator_Positron_HitPosition_x", &PlasticScintillator_Positron_HitPosition_x, "PlasticScintillator_Positron_HitPosition_x/D");
    Tree_Positron->Branch("PlasticScintillator_Positron_HitPosition_y", &PlasticScintillator_Positron_HitPosition_y, "PlasticScintillator_Positron_HitPosition_y/D");
    Tree_Positron->Branch("PlasticScintillator_Positron_HitPosition_z", &PlasticScintillator_Positron_HitPosition_z, "PlasticScintillator_Positron_HitPosition_z/D");

    Tree_Electron = new TTree("Tree_Electron", "Information Electron");
    Tree_Electron->Branch("Event_Number", &Event_Number, "Event_Number/I");
    Tree_Electron->Branch("Initial_Electron_Momentum_x", &Initial_Electron_Momentum_x, "Initial_Electron_Momentum_x/D");
    Tree_Electron->Branch("Initial_Electron_Momentum_y", &Initial_Electron_Momentum_y, "Initial_Electron_Momentum_y/D");
    Tree_Electron->Branch("Initial_Electron_Momentum_z", &Initial_Electron_Momentum_z, "Initial_Electron_Momentum_z/D");
    Tree_Electron->Branch("Initial_Electron_Energy", &Initial_Electron_Energy, "Initial_Electron_Energy/D");
    Tree_Electron->Branch("Catcher_Electron_EnergyDeposit", &Catcher_Electron_EnergyDeposit, "Catcher_Electron_EnergyDeposit/D");
    Tree_Electron->Branch("PlasticScintillator_Electron_EnergyDeposit", &PlasticScintillator_Electron_EnergyDeposit, "PlasticScintillator_Electron_EnergyDeposit/D");
    Tree_Electron->Branch("PlasticScintillator_Electron_HitAngle", &PlasticScintillator_Electron_HitAngle, "PlasticScintillator_Electron_HitAngle/D");
    Tree_Electron->Branch("PlasticScintillator_Electron_HitPosition_x", &PlasticScintillator_Electron_HitPosition_x, "PlasticScintillator_Electron_HitPosition_x/D");
    Tree_Electron->Branch("PlasticScintillator_Electron_HitPosition_y", &PlasticScintillator_Electron_HitPosition_y, "PlasticScintillator_Electron_HitPosition_y/D");
    Tree_Electron->Branch("PlasticScintillator_Electron_HitPosition_z", &PlasticScintillator_Electron_HitPosition_z, "PlasticScintillator_Electron_HitPosition_z/D");

    Tree_Gamma = new TTree("Tree_Gamma", "Information Gamma");
    Tree_Gamma->Branch("Event_Number", &Event_Number, "Event_Number/I");
    Tree_Gamma->Branch("Initial_Gamma_Momentum_x", &Initial_Gamma_Momentum_x, "Initial_Gamma_Momentum_x/D");
    Tree_Gamma->Branch("Initial_Gamma_Momentum_y", &Initial_Gamma_Momentum_y, "Initial_Gamma_Momentum_y/D");
    Tree_Gamma->Branch("Initial_Gamma_Momentum_z", &Initial_Gamma_Momentum_z, "Initial_Gamma_Momentum_z/D");
    Tree_Gamma->Branch("Initial_Gamma_Energy", &Initial_Gamma_Energy, "Initial_Gamma_Energy/D");
    Tree_Gamma->Branch("Catcher_Gamma_EnergyDeposit", &Catcher_Gamma_EnergyDeposit, "Catcher_Gamma_EnergyDeposit/D");
    Tree_Gamma->Branch("Silicon_Gamma_EnergyDeposit", &Silicon_Gamma_EnergyDeposit, "Silicon_Gamma_EnergyDeposit/D");
    Tree_Gamma->Branch("Silicon_DL_Gamma_EnergyDeposit", &Silicon_DL_Gamma_EnergyDeposit, "Silicon_DL_Gamma_EnergyDeposit/D");
    Tree_Gamma->Branch("Silicon_Gamma_HitAngle", &Silicon_Gamma_HitAngle, "Silicon_Gamma_HitAngle/D");
    Tree_Gamma->Branch("Silicon_Name_Gamma", &Silicon_Name_Gamma);
    Tree_Gamma->Branch("Silicon_Gamma_HitPosition_x", &Silicon_Gamma_HitPosition_x, "Silicon_Gamma_HitPosition_x/D");
    Tree_Gamma->Branch("Silicon_Gamma_HitPosition_y", &Silicon_Gamma_HitPosition_y, "Silicon_Gamma_HitPosition_y/D");
    Tree_Gamma->Branch("Silicon_Gamma_HitPosition_z", &Silicon_Gamma_HitPosition_z, "Silicon_Gamma_HitPosition_z/D");
    Tree_Gamma->Branch("PlasticScintillator_Gamma_EnergyDeposit", &PlasticScintillator_Gamma_EnergyDeposit, "PlasticScintillator_Gamma_EnergyDeposit/D");
    Tree_Gamma->Branch("PlasticScintillator_Gamma_HitAngle", &PlasticScintillator_Gamma_HitAngle, "PlasticScintillator_Gamma_HitAngle/D");
    Tree_Gamma->Branch("PlasticScintillator_Gamma_HitPosition_x", &PlasticScintillator_Gamma_HitPosition_x, "PlasticScintillator_Gamma_HitPosition_x/D");
    Tree_Gamma->Branch("PlasticScintillator_Gamma_HitPosition_y", &PlasticScintillator_Gamma_HitPosition_y, "PlasticScintillator_Gamma_HitPosition_y/D");
    Tree_Gamma->Branch("PlasticScintillator_Gamma_HitPosition_z", &PlasticScintillator_Gamma_HitPosition_z, "PlasticScintillator_Gamma_HitPosition_z/D");
    ///////////////////////////////////////////////////////////////////
  }
  // call the base class function (whatever it is supposed to do)
  G4RunManager::AnalyzeEvent(event);

  G4PrimaryVertex *PrimaryVertex = event->GetPrimaryVertex();
  if (PrimaryVertex != 0)
  {
    count++;
    Catcher_EnergyDeposit = 0;
    PlasticScintillator_EnergyDeposit = 0;
    Silicon_EnergyDeposit = 0;
    Silicon_DL_EnergyDeposit = 0;
    x = PrimaryVertex->GetX0() / um;
    y = PrimaryVertex->GetY0() / um;
    z = PrimaryVertex->GetZ0() / nm;
    Event_Number = event->GetEventID();

    G4bool detected = false;
    for (int i = 0; i < nb_det; i++)
    {
      for (int part = 1; part <= event->GetNumberOfPrimaryVertex(); part++)
      {
        PrimaryInfo dic_det = dic_detector[Detector_Name[i]].first->GetDictionnary()[part];
        PrimaryInfo dic_dl = dic_detector[Detector_Name[i]].second->GetDictionnary()[part];
        if (dic_detector[Detector_Name[i]].first->GetDictionnary()[part].DepositEnergy != 0)
        {
          Silicon_Name = Detector_Name[i];
          Silicon_EnergyDeposit += dic_det.DepositEnergy;
          Silicon_DL_EnergyDeposit += dic_dl.DepositEnergy;

          detected = true;
        }
      }
      Tree_Common->Fill();
    }
    if (!detected)
    {
      Tree_Common->Fill();
    }
  

    for (int part = 1; part <= event->GetNumberOfPrimaryVertex(); part++)
    {
      G4PrimaryParticle *Primary = event->GetPrimaryVertex(part - 1)->GetPrimary();
      G4ThreeVector Momentum = Primary->GetMomentumDirection();

      Catcher_EnergyDeposit += wisard_sensor_CatcherMylar->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl1->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl2->GetDictionnary()[part].DepositEnergy;
      PlasticScintillator_EnergyDeposit += wisard_sensor_CatcherMylar->GetDictionnary()[part].DepositEnergy;

      if (Primary->GetG4code()->GetParticleName() == "proton")
      {
        detected = false;
        Initial_Proton_Energy = Primary->GetKineticEnergy() / keV;
        Initial_Proton_Momentum_x = Momentum.x();
        Initial_Proton_Momentum_y = Momentum.y();
        Initial_Proton_Momentum_z = Momentum.z();
        Catcher_Proton_EnergyDeposit = wisard_sensor_CatcherMylar->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl1->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl2->GetDictionnary()[part].DepositEnergy;
        PlasticScintillator_Proton_EnergyDeposit = wisard_sensor_PlasticScintillator->GetDictionnary()[part].DepositEnergy;
        for (int i = 0; i < nb_det; i++)
        {
          PrimaryInfo dic_det = dic_detector[Detector_Name[i]].first->GetDictionnary()[part];
          PrimaryInfo dic_dl = dic_detector[Detector_Name[i]].second->GetDictionnary()[part];
          if (dic_detector[Detector_Name[i]].first->GetDictionnary()[part].DepositEnergy != 0)
          {
            Silicon_Name_Proton = Detector_Name[i];
            Silicon_Proton_EnergyDeposit = dic_det.DepositEnergy;
            Silicon_DL_Proton_EnergyDeposit = dic_dl.DepositEnergy;
            Silicon_Proton_HitAngle = dic_det.HitAngle;
            Silicon_Proton_HitPosition_x = dic_det.HitPosition.x();
            Silicon_Proton_HitPosition_y = dic_det.HitPosition.y();
            Silicon_Proton_HitPosition_z = dic_det.HitPosition.z();
            Tree_Proton->Fill();

            detected = true;
          }
        }
        if (!detected)
        {
          Tree_Proton->Fill();
        }
      }

      if (Primary->GetG4code()->GetParticleName() == "alpha")
      {
        detected = false;
        Initial_Alpha_Energy = Primary->GetKineticEnergy() / keV;
        Initial_Alpha_Momentum_x = Momentum.x();
        Initial_Alpha_Momentum_y = Momentum.y();
        Initial_Alpha_Momentum_z = Momentum.z();
        Catcher_Alpha_EnergyDeposit = wisard_sensor_CatcherMylar->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl1->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl2->GetDictionnary()[part].DepositEnergy;
        PlasticScintillator_Alpha_EnergyDeposit = wisard_sensor_PlasticScintillator->GetDictionnary()[part].DepositEnergy;
        for (int i = 0; i < nb_det; i++)
        {
          PrimaryInfo dic_det = dic_detector[Detector_Name[i]].first->GetDictionnary()[part];
          PrimaryInfo dic_dl = dic_detector[Detector_Name[i]].second->GetDictionnary()[part];
          if (dic_detector[Detector_Name[i]].first->GetDictionnary()[part].DepositEnergy != 0)
          {
            Silicon_Name_Alpha = Detector_Name[i];
            Silicon_Alpha_EnergyDeposit = dic_det.DepositEnergy;
            Silicon_DL_Alpha_EnergyDeposit = dic_dl.DepositEnergy;
            Silicon_Alpha_HitAngle = dic_dl.HitAngle;
            Silicon_Alpha_HitPosition_x = dic_det.HitPosition.x();
            Silicon_Alpha_HitPosition_y = dic_det.HitPosition.y();
            Silicon_Alpha_HitPosition_z = dic_det.HitPosition.z();
            Tree_Alpha->Fill();

            detected = true;
          }
        }
        if (!detected)
        {
          Tree_Alpha->Fill();
        }
      }

      if (Primary->GetG4code()->GetParticleName() == "e+")
      {
        Initial_Positron_Energy = Primary->GetKineticEnergy() / keV;
        Initial_Positron_Momentum_x = Momentum.x();
        Initial_Positron_Momentum_y = Momentum.y();
        Initial_Positron_Momentum_z = Momentum.z();
        Catcher_Positron_EnergyDeposit = wisard_sensor_CatcherMylar->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl1->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl2->GetDictionnary()[part].DepositEnergy;

        PrimaryInfo PlasticScintillator = wisard_sensor_PlasticScintillator->GetDictionnary()[part];
        PlasticScintillator_Positron_EnergyDeposit = PlasticScintillator.DepositEnergy;
        PlasticScintillator_Positron_HitAngle = PlasticScintillator.HitAngle;
        PlasticScintillator_Positron_HitPosition_x = PlasticScintillator.HitPosition.x();
        PlasticScintillator_Positron_HitPosition_y = PlasticScintillator.HitPosition.y();
        PlasticScintillator_Positron_HitPosition_z = PlasticScintillator.HitPosition.z();

        Tree_Positron->Fill();
      }

      if (Primary->GetG4code()->GetParticleName() == "e-")
      {
        Initial_Electron_Energy = Primary->GetKineticEnergy() / keV;
        Initial_Electron_Momentum_x = Momentum.x();
        Initial_Electron_Momentum_y = Momentum.y();
        Initial_Electron_Momentum_z = Momentum.z();
        Catcher_Electron_EnergyDeposit = wisard_sensor_CatcherMylar->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl1->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl2->GetDictionnary()[part].DepositEnergy;

        PrimaryInfo PlasticScintillator = wisard_sensor_PlasticScintillator->GetDictionnary()[part];
        PlasticScintillator_Electron_EnergyDeposit = PlasticScintillator.DepositEnergy;
        PlasticScintillator_Electron_HitAngle = PlasticScintillator.HitAngle;
        PlasticScintillator_Electron_HitPosition_x = PlasticScintillator.HitPosition.x();
        PlasticScintillator_Electron_HitPosition_y = PlasticScintillator.HitPosition.y();
        PlasticScintillator_Electron_HitPosition_z = PlasticScintillator.HitPosition.z();

        Tree_Electron->Fill();
      }
      if (Primary->GetG4code()->GetParticleName() == "gamma")
      {
        detected = false;
        Initial_Gamma_Energy = Primary->GetKineticEnergy() / keV;
        Initial_Gamma_Momentum_x = Momentum.x();
        Initial_Gamma_Momentum_y = Momentum.y();
        Initial_Gamma_Momentum_z = Momentum.z();
        Catcher_Gamma_EnergyDeposit = wisard_sensor_CatcherMylar->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl1->GetDictionnary()[part].DepositEnergy + wisard_sensor_CatcherAl2->GetDictionnary()[part].DepositEnergy;
        PrimaryInfo PlasticScintillator = wisard_sensor_PlasticScintillator->GetDictionnary()[part];
        PlasticScintillator_Electron_EnergyDeposit = PlasticScintillator.DepositEnergy;
        PlasticScintillator_Electron_HitAngle = PlasticScintillator.HitAngle;
        PlasticScintillator_Electron_HitPosition_x = PlasticScintillator.HitPosition.x();
        PlasticScintillator_Electron_HitPosition_y = PlasticScintillator.HitPosition.y();
        PlasticScintillator_Electron_HitPosition_z = PlasticScintillator.HitPosition.z();
        for (int i = 0; i < nb_det; i++)
        {
          PrimaryInfo dic_det = dic_detector[Detector_Name[i]].first->GetDictionnary()[part];
          PrimaryInfo dic_dl = dic_detector[Detector_Name[i]].second->GetDictionnary()[part];
          if (dic_detector[Detector_Name[i]].first->GetDictionnary()[part].DepositEnergy != 0)
          {
            Silicon_Name_Gamma = Detector_Name[i];
            Silicon_Gamma_EnergyDeposit = dic_det.DepositEnergy;
            Silicon_DL_Gamma_EnergyDeposit = dic_dl.DepositEnergy;
            Silicon_Gamma_HitAngle = dic_dl.HitAngle;
            Silicon_Gamma_HitPosition_x = dic_det.HitPosition.x();
            Silicon_Gamma_HitPosition_y = dic_det.HitPosition.y();
            Silicon_Gamma_HitPosition_z = dic_det.HitPosition.z();
            Tree_Gamma->Fill();

            detected = true;
          }
        }
        if (!detected)
        {
          Tree_Gamma->Fill();
        }
      }
    }
  }

  //HISTOGRAMS ///#condition à revoir pour plus précis et générale
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
        if (wisard_sensor_PlasticScintillator->GetDictionnary()[index_beta].DepositEnergy > GetThreshoold())
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

  int divi = 100;

  ///// Writing all trees ///////////////////////////////////////////
  if (count % divi == 0)
  {
    Tree_Common->AutoSave("FlushBaskets");

    if (Tree_Alpha->GetEntries() != 0)
    {
      Tree_Alpha->AutoSave("FlushBaskets");
    }
    if (Tree_Proton->GetEntries() != 0)
    {
      Tree_Proton->AutoSave("FlushBaskets");
    }
    if (Tree_Positron->GetEntries() != 0)
    {
      Tree_Positron->AutoSave("FlushBaskets");
    }
    if (Tree_Electron->GetEntries() != 0)
    {
      Tree_Electron->AutoSave("FlushBaskets");
    }
    if (Tree_Gamma->GetEntries() != 0)
    {
      Tree_Gamma->AutoSave("FlushBaskets");
    }

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

    dic_detector[Detector_Name[i]].first->ResetEventEnergy();
    dic_detector[Detector_Name[i]].second->ResetEventEnergy();
  }
  wisard_sensor_CatcherMylar->ResetDictionnary();
  wisard_sensor_CatcherAl1->ResetDictionnary();
  wisard_sensor_CatcherAl2->ResetDictionnary();
  wisard_sensor_PlasticScintillator->ResetEventEnergy_positron();
  ///////////////////////////////////////////////////////////////////

  ///// Reset all variables of detectors ////////////////////////////
  x = 0;
  y = 0;
  z = 0;
  Catcher_EnergyDeposit = 0;
  PlasticScintillator_EnergyDeposit = 0;
  Silicon_EnergyDeposit = 0;
  Silicon_DL_EnergyDeposit = 0;
  Silicon_Name = "";

  Initial_Proton_Momentum_x = 0;
  Initial_Proton_Momentum_y = 0;
  Initial_Proton_Momentum_z = 0;
  Initial_Proton_Energy = 0;
  Catcher_Proton_EnergyDeposit = 0;
  PlasticScintillator_Proton_EnergyDeposit = 0;
  Silicon_Proton_EnergyDeposit = 0;
  Silicon_DL_Proton_EnergyDeposit = 0;
  Silicon_Proton_HitAngle = 0;
  Silicon_Name_Proton = "";
  Silicon_Proton_HitPosition_x = 0;
  Silicon_Proton_HitPosition_y = 0;
  Silicon_Proton_HitPosition_z = 0;

  Initial_Alpha_Momentum_x = 0;
  Initial_Alpha_Momentum_y = 0;
  Initial_Alpha_Momentum_z = 0;
  Initial_Alpha_Energy = 0;
  Catcher_Alpha_EnergyDeposit = 0;
  PlasticScintillator_Alpha_EnergyDeposit = 0;
  Silicon_Alpha_EnergyDeposit = 0;
  Silicon_DL_Alpha_EnergyDeposit = 0;
  Silicon_Alpha_HitAngle = 0;
  Silicon_Name_Alpha = "";
  Silicon_Alpha_HitPosition_x = 0;
  Silicon_Alpha_HitPosition_y = 0;
  Silicon_Alpha_HitPosition_z = 0;

  Initial_Positron_Momentum_x = 0;
  Initial_Positron_Momentum_y = 0;
  Initial_Positron_Momentum_z = 0;
  Initial_Positron_Energy = 0;
  Catcher_Positron_EnergyDeposit = 0;
  PlasticScintillator_Positron_EnergyDeposit = 0;
  PlasticScintillator_Positron_HitAngle = 0;
  PlasticScintillator_Positron_HitPosition_x = 0;
  PlasticScintillator_Positron_HitPosition_y = 0;
  PlasticScintillator_Positron_HitPosition_z = 0;

  Initial_Electron_Momentum_x = 0;
  Initial_Electron_Momentum_y = 0;
  Initial_Electron_Momentum_z = 0;
  Initial_Electron_Energy = 0;
  Catcher_Electron_EnergyDeposit = 0;
  PlasticScintillator_Electron_EnergyDeposit = 0;
  PlasticScintillator_Electron_HitAngle = 0;
  PlasticScintillator_Electron_HitPosition_x = 0;
  PlasticScintillator_Electron_HitPosition_y = 0;
  PlasticScintillator_Electron_HitPosition_z = 0;

  Initial_Gamma_Momentum_x = 0;
  Initial_Gamma_Momentum_y = 0;
  Initial_Gamma_Momentum_z = 0;
  Initial_Gamma_Energy = 0;
  Catcher_Gamma_EnergyDeposit = 0;
  Silicon_Gamma_EnergyDeposit = 0;
  Silicon_DL_Gamma_EnergyDeposit = 0;
  Silicon_Gamma_HitAngle = 0;
  Silicon_Name_Gamma = "";
  Silicon_Gamma_HitPosition_x = 0;
  Silicon_Gamma_HitPosition_y = 0;
  Silicon_Gamma_HitPosition_z = 0;
  PlasticScintillator_Gamma_EnergyDeposit = 0;
  PlasticScintillator_Gamma_HitAngle = 0;
  PlasticScintillator_Gamma_HitPosition_x = 0;
  PlasticScintillator_Gamma_HitPosition_y = 0;
  PlasticScintillator_Gamma_HitPosition_z = 0;
  ////////////////////////////////////////////////////////

  //////// Resolution on detectors ///////////////////////
  //e_PlasticScintillator = max(e_PlasticScintillator+CLHEP::RandGauss::shoot(resolution_simps, 0.0));

  double mean = 0.;
  double std = 0.;
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