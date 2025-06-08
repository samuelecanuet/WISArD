#include "Wisard_RunAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

Wisard_RunAction::Wisard_RunAction(G4String macrofilename) : G4UserRunAction(), MacroFileName(macrofilename)
{
  RunMessenger = new G4GenericMessenger(this, "/Run/", "Output Settings");

  RunMessenger->DeclareProperty("File", filename)
      .SetGuidance("Set ROOT Output filename")
      .SetParameterName("Filename", false)
      .SetDefaultValue("output.root");

  RunMessenger->DeclareProperty("Threads", dummy)
      .SetGuidance("Taking value to avoid error")
      .SetParameterName("Thread", false)
      .SetDefaultValue("1");

  RunMessenger->DeclarePropertyWithUnit("Threshold_PlasticScintillator", "keV", threshold)
      .SetGuidance("Set the threshold for the detectors")
      .SetParameterName("Threshold", false)
      .SetDefaultValue("100.");
}
Wisard_RunAction::~Wisard_RunAction() {}

void Wisard_RunAction::BeginOfRunAction(const G4Run *)
{
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4String fn = filename.substr(0, filename.length() - 5);
  G4int thread = G4Threading::G4GetThreadId();

  f = new TFile((fn + "_" + to_string(thread) + ".root").c_str(), "recreate");
  ////////////// Construct Log /////////////////////////////////////
  std::ifstream macrofile(MacroFileName);
  std::string line;
  while (std::getline(macrofile, line))
  {
    if ((line.find("/Geometry") != std::string::npos || line.find("/Input") != std::string::npos || line.find("/Run") != std::string::npos || line.find("/Beam") != std::string::npos) && line.find("#") == std::string::npos)
    {
      char const *num_char = line.c_str();
      TObjString objString(line.c_str());
      f->WriteObject(&objString, num_char);
    }
    f->cd();
  }

  ////////////// Construct Tree ///////////////////////////////////
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
  Tree->Branch("Silicon_Detector_Hit_Time", &Silicon_Detector_Hit_Time);
  Tree->Branch("Silicon_Detector_Code", &Silicon_Detector_Code);
  Tree->Branch("Silicon_Detector_DL_Energy_Deposit", &Silicon_Detector_DL_Energy_Deposit);

  ////////////// Construct Histograms ////////////////////////////
  for (int i = 0; i < Wisard_Detector::nb_det; i++)
  {
    G4String name = Detector_Name[i];
    G4int detector_code = Detector_Code[i];
    silicon_coinc[detector_code] = new TH1D((name + "_coinc").c_str(), (name + "_coinc").c_str(), 100000, 0.0, 10000.0);
    silicon_nocoinc[detector_code] = new TH1D((name + "_nocoinc").c_str(), (name + "_nocoinc").c_str(), 100000, 0.0, 10000.0);
    silicon_single[detector_code] = new TH1D((name + "_single").c_str(), (name + "_single").c_str(), 100000, 0.0, 10000.0);
  }
  plastic_coinc = new TH1D("plastic_coinc", "plastic_coinc", 120000, 0.0, 12000.0);
}

void Wisard_RunAction::EndOfRunAction(const G4Run *)
{
  WrittingTree();
  f->Close();
}

void Wisard_RunAction::UpdateTree(ParticleInformation *Part_Info)
{
  // Part_Info->Parse();

  // TREE VARIABLES //
  for (auto &pair : Part_Info->GetInfo())
  {
    Particle particle = pair.second;

    Particle_PDG.push_back(particle.Particle_PDG);
    x.push_back(particle.Pos.x());
    y.push_back(particle.Pos.y());
    z.push_back(particle.Pos.z());
    px.push_back(particle.Dir.x());
    py.push_back(particle.Dir.y());
    pz.push_back(particle.Dir.z());
    T0.push_back(particle.Time);
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
        silicon_single[Det.first]->Fill(Det.second.EnergyDeposit);
        Silicon_Detector_Energy_Deposit_part.push_back(Det.second.EnergyDeposit);
        Silicon_Detector_Hit_Position_part.push_back(Det.second.HitPosition);
        Silicon_Detector_Hit_Angle_part.push_back(Det.second.HitAngle);
        Silicon_Detector_Hit_Time_part.push_back(Det.second.HitTime);
      }
      // DL
      else if (Det.first < 1000 && Det.first > 100)
      {
        Silicon_Detector_Code_part.push_back(Det.first);
        Silicon_Detector_DL_Energy_Deposit_part.push_back(Det.second.EnergyDeposit);
      }
    }

    Silicon_Detector_Code.push_back(Silicon_Detector_Code_part);
    Silicon_Detector_Energy_Deposit.push_back(Silicon_Detector_Energy_Deposit_part);
    Silicon_Detector_DL_Energy_Deposit.push_back(Silicon_Detector_DL_Energy_Deposit_part);
    Silicon_Detector_Hit_Position.push_back(Silicon_Detector_Hit_Position_part);
    Silicon_Detector_Hit_Angle.push_back(Silicon_Detector_Hit_Angle_part);
    Silicon_Detector_Hit_Time.push_back(Silicon_Detector_Hit_Time_part);

    Silicon_Detector_Code_part.clear();
    Silicon_Detector_Energy_Deposit_part.clear();
    Silicon_Detector_DL_Energy_Deposit_part.clear();
    Silicon_Detector_Hit_Position_part.clear();
    Silicon_Detector_Hit_Angle_part.clear();
    Silicon_Detector_Hit_Time_part.clear();
  }
  Tree->Fill();

  Particle_PDG.clear();
  x.clear();
  y.clear();
  z.clear();
  px.clear();
  py.clear();
  pz.clear();
  T0.clear();
  Kinetic_Energy.clear();
  Silicon_Detector_Code.clear();
  Silicon_Detector_Energy_Deposit.clear();
  Silicon_Detector_DL_Energy_Deposit.clear();
  Silicon_Detector_Hit_Position.clear();
  Silicon_Detector_Hit_Angle.clear();
  Silicon_Detector_Hit_Time.clear();
  PlasticScintillator_Energy_Deposit.clear();
  PlasticScintillator_Hit_Angle.clear();
  PlasticScintillator_Hit_Position.clear();
  PlasticScintillator_Hit_Time.clear();
  Catcher_Central_Energy_Deposit.clear();
  Catcher_Side_Energy_Deposit.clear();

  

  /// HISTOGRAMS ///
  //  Init
  /*
  for (auto &pair : Part_Info->GetInfo())
  {
    Particle particle = pair.second;
    G4int PDG = particle.Particle_PDG;

    if (PDG > 1000000000)
    {
      PDG = (PDG / 10) * 10;
    }

    if (H_E0.find(PDG) == H_E0.end())
    {
      G4String name = G4ParticleTable::GetParticleTable()->FindParticle(PDG)->GetParticleName();
      if (PDG > 1000000000)
      {
        H_E0[PDG] = new TH1D(("E0_" + name).c_str(), ("E0_" + name).c_str(), 1000, 0.0, 1000);
      }
      else
        H_E0[PDG] = new TH1D(("E0_" + name).c_str(), ("E0_" + name).c_str(), 10000, 0.0, 10000);
    }
    H_E0[PDG]->Fill(particle.E0);
  }

  // Detectors
  int proton_index = -1;
  int positron_index = -1;
  for (auto &pair : Part_Info->GetInfo())
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

  // if (proton_index != -1 && positron_index != -1)
  // {
  for (int i = 0; i < Wisard_Detector::nb_det; i++)
  {
    Detector Det = (Part_Info->GetInfo()[proton_index]).Detectors[Detector_Code[i]];
    if (!Det.EnergyDeposit)
      continue;

    if (Part_Info->GetInfo()[positron_index].Detectors[99].EnergyDeposit >= threshold)
    {
      silicon_coinc[i]->Fill(Det.EnergyDeposit);
      plastic_coinc->Fill((Part_Info->GetInfo()[positron_index]).Detectors[99].EnergyDeposit);
    }
    else
    {
      silicon_nocoinc[i]->Fill(Det.EnergyDeposit);
    }
    silicon_single[i]->Fill(Det.EnergyDeposit);
  }
  // }
  */

  G4int divi = 100000;
  G4int EventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  if (EventID % divi == 0)
  {
    WrittingTree();
  }
}

void Wisard_RunAction::WrittingTree()
{
  // G4cout << "Writing Tree" << G4endl; 
  f->cd();


  Tree->AutoSave("FlushBaskets");

  for (int i = 0; i < Wisard_Detector::nb_det; i++)
  {
    G4int detector_code = Detector_Code[i];
    // G4cout<< Wisard_Detector::Detector_Name[i] << " : " << silicon_coinc[i]->GetEntries() << " " << silicon_nocoinc[i]->GetEntries() << " " << silicon_single[i]->GetEntries() <<G4endl;
    silicon_coinc[detector_code]->Write("", TObject::kOverwrite);
    silicon_nocoinc[detector_code]->Write("", TObject::kOverwrite);
    silicon_single[detector_code]->Write("", TObject::kOverwrite);
  }
  plastic_coinc->Write("", TObject::kOverwrite);

  for (auto pair : H_E0)
  {
    pair.second->Write("", TObject::kOverwrite);
  }
}

G4String Wisard_RunAction::GetFileName()
{
  return filename;
}
