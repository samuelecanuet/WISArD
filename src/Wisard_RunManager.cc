#include "Wisard_RunManager.hh"
#include "Wisard_Sensor.hh"


#include "G4UImanager.hh"

#include "TH1D.h"
#include "TObjString.h"

#include <iostream>
//----------------------------------------------------------------------

// constructor
Wisard_RunManager::Wisard_RunManager(ParticleInformation* PartInfos) : PartInfo(PartInfos)
{ 
  G4cout << "\033[32m" << "Constructor Wisard_RunManager" << "\033[0m" << G4endl;
  G4cout << "\033[32m" << "Constructor Wisard_Sensor" << "\033[0m" << G4endl;
  ////////////// Construct all sensors //////////////////////////////
 
  wisard_sensor_PlasticScintillator = new Wisard_Sensor(PartInfo, 99);
  for (int i = 0; i < nb_det; i++)
  {
    dic_detector[Detector_Code[i]] = std::make_pair(new Wisard_Sensor(PartInfo, Detector_Code[i]), new Wisard_Sensor(PartInfo, Detector_Code[i]*10));
  }
  for (int i : InterDetector_Code)
  {
    dic_interdetector[i] = new Wisard_Sensor(PartInfo, i);
  }
  wisard_sensor_CatcherMylar_central = new Wisard_Sensor(PartInfo, 1);
  wisard_sensor_CatcherAl1_central = new Wisard_Sensor(PartInfo, 2);
  wisard_sensor_CatcherAl2_central = new Wisard_Sensor(PartInfo, 3);
  wisard_sensor_CatcherMylar_side = new Wisard_Sensor(PartInfo, 4);
  wisard_sensor_CatcherAl1_side = new Wisard_Sensor(PartInfo, 5);
  wisard_sensor_CatcherAl2_side = new Wisard_Sensor(PartInfo, 6);

  wisard_killer = new Wisard_Killer();
  ///////////////////////////////////////////////////////////////////

  ////////////// Construct strips coinc/anticoinc HISTOGRAMS ////////
  for (int i = 0; i < nb_det; i++)
  {
    silicon_coinc[i] = new TH1D((Detector_Name[i] + "_coinc").c_str(), (Detector_Name[i] + "_coinc").c_str(), 100000, 0.0, 10000.0);
    silicon_nocoinc[i] = new TH1D((Detector_Name[i] + "_nocoinc").c_str(), (Detector_Name[i] + "_nocoinc").c_str(), 100000, 0.0, 10000.0);
    silicon_single[i] = new TH1D((Detector_Name[i] + "_single").c_str(), (Detector_Name[i] + "_single").c_str(), 100000, 0.0, 10000.0);
  }
  plastic_coinc = new TH1D("plastic_coinc", "plastic_coinc", 120000, 0.0, 12000.0);
  ///////////////////////////////////////////////////////////////////

  // Messenger //
  RunMessenger = new G4GenericMessenger(this, "/Run/", "Output Settings");

  RunMessenger->DeclareProperty("File", filename)
      .SetGuidance("Set ROOT Output filename")
      .SetParameterName("Filename", false)
      .SetDefaultValue("output.root");

  RunMessenger->DeclarePropertyWithUnit("Threshold_PlasticScintillator", "keV", threshold)
      .SetGuidance("Set the threshold for the detectors")
      .SetParameterName("Threshold", false)
      .SetDefaultValue("100.");
}


Wisard_RunManager::~Wisard_RunManager()
{
  Tree->AutoSave("FlushBaskets");
 G4cout << "\033[31m" << "Destructor Wisard_RunManager"  << "\033[0m" << G4endl;

  f->Close();
  delete f;

 G4cout << "\033[31m" << "Destructor Wisard_Sensor"  << "\033[0m" << G4endl;
  delete wisard_sensor_PlasticScintillator;
  for (int i = 0; i < nb_det; i++)
  {
    delete dic_detector[Detector_Code[i]].first;
    delete dic_detector[Detector_Code[i]].second;
  }
  for (int i : InterDetector_Code)
  {
    delete dic_interdetector[i];
  }
  delete wisard_sensor_CatcherMylar_central;
  delete wisard_sensor_CatcherAl1_central;
  delete wisard_sensor_CatcherAl2_central;
  delete wisard_sensor_CatcherMylar_side;
  delete wisard_sensor_CatcherAl1_side;
  delete wisard_sensor_CatcherAl2_side;
}

//----------------------------------------------------------------------
void Wisard_RunManager::AnalyzeEvent(G4Event *event)
{  
  if (event->GetEventID() == 0)
  {
    f = new TFile(filename, "recreate");
    ////////////// Construct Log /////////////////////////////////////
    // TDirectory *dir = f->mkdir("Log");
    // dir->cd();
    G4UImanager *uiManager = G4UImanager::GetUIpointer();
    for (int i = 0; i < uiManager->GetNumberOfHistory(); i++)
    {
      G4String command = uiManager->GetPreviousCommand(i).substr(1);
      if (command.find("run") != 0 && command.find("event") != 0 && command.find("tracking") != 0 && command.find("Input") != 0 && command.find("Run") != 0 && command.find("vis") != 0 && command.find("process") != 0)
      {
        char const *num_char = command.c_str();
        TObjString objString(command.c_str());
        f->WriteObject(&objString, num_char);
      }
      f->cd();
    }
    //////////////////////////////////////////////////////////////////

    ////////////// Construct the TREE ////////////////////////////////
    Tree = new TTree("Tree", "Informations");
    Tree->Branch("Particle_PDG", &Particle_PDG);
    Tree->Branch("x", &x);
    Tree->Branch("y", &y);
    Tree->Branch("z", &z);
    Tree->Branch("px", &px);
    Tree->Branch("py", &py);
    Tree->Branch("pz", &pz);
    Tree->Branch("T0", &T0);
    Tree->Branch("Kinetic_Energy", &Kinetic_Energy);
    Tree->Branch("Catcher_Central_Energy_Deposit", &Catcher_Central_Energy_Deposit);
    Tree->Branch("Catcher_Side_Energy_Deposit", &Catcher_Side_Energy_Deposit);
    Tree->Branch("PlasticScintillator_Energy_Deposit", &PlasticScintillator_Energy_Deposit);
    Tree->Branch("PlasticScintillator_Hit_Position", &PlasticScintillator_Hit_Position);
    Tree->Branch("PlasticScintillator_Hit_Angle", &PlasticScintillator_Hit_Angle);
    Tree->Branch("PlasticScintillator_Hit_Time", &PlasticScintillator_Hit_Time);
    Tree->Branch("Silicon_Detector_Energy_Deposit", &Silicon_Detector_Energy_Deposit);
    Tree->Branch("Silicon_Detector_Hit_Position", &Silicon_Detector_Hit_Position);
    Tree->Branch("Silicon_Detector_Hit_Angle", &Silicon_Detector_Hit_Angle);
    Tree->Branch("Silicon_Detector_HitTime", &Silicon_Detector_HitTime);
    Tree->Branch("Silicon_Detector_Code", &Silicon_Detector_Code);
    Tree->Branch("Silicon_Detector_DL_Energy_Deposit", &Silicon_Detector_DL_Energy_Deposit);
    Tree->Branch("Silicon_Detector_InterStrip_Code", &Silicon_Detector_InterStrip_Code);
    Tree->Branch("Silicon_Detector_InterStrip_Energy_Deposit", &Silicon_Detector_InterStrip_Energy_Deposit);
    Tree->Branch("Silicon_Detector_InterStrip_Hit_Position", &Silicon_Detector_InterStrip_Hit_Position);
    ///////////////////////////////////////////////////////////////////
  }

  G4RunManager::AnalyzeEvent(event);

  //new 
  for (auto& pair : PartInfo->GetInfo())
  {
    Particle particle = pair.second;

    Particle_PDG.push_back(particle.Particle_PDG);
    x.push_back(particle.Pos.x());
    y.push_back(particle.Pos.y());
    z.push_back(particle.Pos.z());
    px.push_back(particle.Dir.x());
    py.push_back(particle.Dir.y());
    pz.push_back(particle.Dir.z());     
    Kinetic_Energy.push_back(particle.E0);


    // # Catcher Central #//
   G4double catcher_central = 0;
    if (particle.Detectors.find(1) != particle.Detectors.end())
      catcher_central += particle.Detectors[1].EnergyDeposit;
    if (particle.Detectors.find(2) != particle.Detectors.end())
      catcher_central += particle.Detectors[2].EnergyDeposit;
    if (particle.Detectors.find(3) != particle.Detectors.end())
      catcher_central += particle.Detectors[3].EnergyDeposit;
    Catcher_Central_Energy_Deposit.push_back(catcher_central);

    // # Catcher Side #//
   G4double catcher_side = 0;
    if (particle.Detectors.find(4) != particle.Detectors.end())
      catcher_side += particle.Detectors[4].EnergyDeposit;
    if (particle.Detectors.find(5) != particle.Detectors.end())
      catcher_side += particle.Detectors[5].EnergyDeposit;
    if (particle.Detectors.find(6) != particle.Detectors.end())
      catcher_side += particle.Detectors[6].EnergyDeposit;
    Catcher_Side_Energy_Deposit.push_back(catcher_side);
  
    // # Plastic Scintillator #//
    PlasticScintillator_Energy_Deposit.push_back(particle.Detectors[99].EnergyDeposit);
    PlasticScintillator_Hit_Position.push_back(particle.Detectors[99].HitPosition);
    PlasticScintillator_Hit_Angle.push_back(particle.Detectors[99].HitAngle);
    PlasticScintillator_Hit_Time.push_back(particle.Detectors[99].HitTime);

    // # Silicon Detectors #//
    for (auto Det : particle.Detectors)
    {
      // Strip
      if (Det.first < 87 && Det.first > 10) 
      {
        Silicon_Detector_Code_part.push_back(Det.first);
        Silicon_Detector_Energy_Deposit_part.push_back(Det.second.EnergyDeposit);
        Silicon_Detector_Hit_Position_part.push_back(Det.second.HitPosition);
        Silicon_Detector_Hit_Angle_part.push_back(Det.second.HitAngle);
        Silicon_Detector_HitTime_part.push_back(Det.second.HitTime);
      }
      // DL
      else if (Det.first < 1000 && Det.first > 100)
      {
        Silicon_Detector_Code_part.push_back(Det.first);
        Silicon_Detector_DL_Energy_Deposit_part.push_back(Det.second.EnergyDeposit);
      }
    }
    // InterStrip
    for (auto interDet : particle.InterStripDetectors)
    {
      Silicon_Detector_InterStrip_Code_part.push_back(interDet.first);
      Silicon_Detector_InterStrip_Energy_Deposit_part.push_back(interDet.second.EnergyDeposit);
      Silicon_Detector_InterStrip_Hit_Position_part.push_back(interDet.second.HitPosition);
    }

    Silicon_Detector_Code.push_back(Silicon_Detector_Code_part);
    Silicon_Detector_Energy_Deposit.push_back(Silicon_Detector_Energy_Deposit_part);
    Silicon_Detector_DL_Energy_Deposit.push_back(Silicon_Detector_DL_Energy_Deposit_part);
    Silicon_Detector_Hit_Position.push_back(Silicon_Detector_Hit_Position_part);
    Silicon_Detector_Hit_Angle.push_back(Silicon_Detector_Hit_Angle_part);
    Silicon_Detector_HitTime.push_back(Silicon_Detector_HitTime_part);

    Silicon_Detector_InterStrip_Code.push_back(Silicon_Detector_InterStrip_Code_part);
    Silicon_Detector_InterStrip_Energy_Deposit.push_back(Silicon_Detector_InterStrip_Energy_Deposit_part);
    Silicon_Detector_InterStrip_Hit_Position.push_back(Silicon_Detector_InterStrip_Hit_Position_part);

    Silicon_Detector_Code_part.clear();
    Silicon_Detector_Energy_Deposit_part.clear();
    Silicon_Detector_DL_Energy_Deposit_part.clear();
    Silicon_Detector_Hit_Position_part.clear();
    Silicon_Detector_Hit_Angle_part.clear();
    Silicon_Detector_HitTime_part.clear();

    Silicon_Detector_InterStrip_Code_part.clear();
    Silicon_Detector_InterStrip_Energy_Deposit_part.clear();
    Silicon_Detector_InterStrip_Hit_Position_part.clear();
  }
  Tree->Fill();
  // PartInfo->Parse();



  /// HISTOGRAMS ///
  //  Init
  for (auto &pair : PartInfo->GetInfo())
  {
    Particle particle = pair.second;
    G4int PDG = particle.Particle_PDG;

    if (PDG > 1000000000)
    {
      PDG = (PDG / 10) * 10;
    }

    if (H_E0.find(PDG) == H_E0.end())
    {
      if (PDG > 1000000000)
      {
        H_E0[PDG] = new TH1D(("E0_" + G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName()).c_str(), ("E0_" + G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName()).c_str(), 1000, 0.0, 1000);
      }
      else
        H_E0[PDG] = new TH1D(("E0_" + G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName()).c_str(), ("E0_" + G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName()).c_str(), 10000, 0.0, 10000);
    }
    H_E0[PDG]->Fill(particle.E0);
  }

  int proton_index = -1;
  int positron_index = -1;
  for (auto& pair : PartInfo->GetInfo())
  {
    if (pair.second.Particle_PDG == 2212 && proton_index == -1)
    {
      proton_index = pair.first;
      break;
    }
    if ((pair.second.Particle_PDG == 11 || pair.second.Particle_PDG == -11) && positron_index == -1)
    {
      positron_index = pair.first;
    }
  }


  if (proton_index != -1 && positron_index != -1)
  {
    for (int i = 0; i < nb_det; i++)
    {
      Detector Det = (PartInfo->GetInfo()[proton_index]).Detectors[Detector_Code[i]];
      if (!Det.EnergyDeposit)
        continue;
      
      if (PartInfo->GetInfo()[positron_index].Detectors[99].EnergyDeposit >= threshold)
      {
        silicon_coinc[i]->Fill(Det.EnergyDeposit);
        plastic_coinc->Fill((PartInfo->GetInfo()[positron_index]).Detectors[99].EnergyDeposit);
      }
      else
      {
        silicon_nocoinc[i]->Fill(Det.EnergyDeposit);
      }
      silicon_single[i]->Fill(Det.EnergyDeposit);
    }
  }

  int divi = 10000;

  ///// Writing in file ///////////////////////////////////////////
  if (event->GetEventID() % divi == 0)
  {
    Tree->AutoSave("FlushBaskets");

    for (int i = 0; i < nb_det; i++)
    {
      //G4cout<< Detector_Name[i] << " : " << silicon_coinc[i]->GetEntries() << " " << silicon_nocoinc[i]->GetEntries() << " " << silicon_single[i]->GetEntries() <<G4endl;
      silicon_coinc[i]->Write("", TObject::kOverwrite);
      silicon_nocoinc[i]->Write("", TObject::kOverwrite);
      silicon_single[i]->Write("", TObject::kOverwrite);
    }
    plastic_coinc->Write("", TObject::kOverwrite);

    for (auto pair : H_E0)
    {
      pair.second->Write("", TObject::kOverwrite);
    }
  }

  ///// Reset all vectors ///////////////////////////////////////////
  Particle_PDG.clear();
  x.clear();
  y.clear();
  z.clear();
  px.clear();
  py.clear();
  pz.clear();
  Kinetic_Energy.clear();
  Silicon_Detector_Code.clear();
  Silicon_Detector_Energy_Deposit.clear();
  Silicon_Detector_DL_Energy_Deposit.clear();
  Silicon_Detector_Hit_Position.clear();
  Silicon_Detector_Hit_Angle.clear();
  Silicon_Detector_HitTime.clear();
  Silicon_Detector_InterStrip_Code.clear();
  Silicon_Detector_InterStrip_Energy_Deposit.clear();
  Silicon_Detector_InterStrip_Hit_Position.clear();
  PlasticScintillator_Energy_Deposit.clear();
  PlasticScintillator_Hit_Angle.clear();
  PlasticScintillator_Hit_Position.clear();
  PlasticScintillator_Hit_Time.clear();
  Catcher_Central_Energy_Deposit.clear();
  Catcher_Side_Energy_Deposit.clear();
  ///////////////////////////////////////////////////////////////////
  PartInfo->Clear();
  ///////////////////////////////////////////////////////////////////

}