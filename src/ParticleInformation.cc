#include "ParticleInformation.hh"
#include "G4TrackingManager.hh"

ParticleInformation::ParticleInformation()
{
}

ParticleInformation::~ParticleInformation()
{
}

void ParticleInformation::SetParticle(G4int TrackID, G4int Particle_PDG, G4double E0, G4ThreeVector Dir, G4ThreeVector Pos, G4double Time)
{
    if (Particles.find(TrackID) == Particles.end())
        AddParticle(TrackID);

    Particles[TrackID].Particle_PDG = Particle_PDG;
    Particles[TrackID].E0 = E0;
    Particles[TrackID].Dir = Dir;
    Particles[TrackID].Pos = Pos;
    Particles[TrackID].Time = Time;
}

void ParticleInformation::AddParticle(G4int TrackID)
{
    Particles[TrackID] = Particle{TrackID, 0, 0, G4ThreeVector(0, 0, 0), G4ThreeVector(0, 0, 0), {}, {}};
}

bool ParticleInformation::FirstHit(G4int TrackID, G4int SensorID)
{

    if (Particles.find(TrackID) == Particles.end())
    {
        AddParticle(TrackID);
    }

    if (Particles[TrackID].Detectors.size() == 0)
    {
        Particles[TrackID].Detectors[SensorID] = Detector{0, 0, G4ThreeVector(0, 0, 0)};
        return true;
    }
    else
    {
        for (auto pair : Particles[TrackID].Detectors)
        {
            if (pair.first == SensorID)
            {
                return false;
            }
        }

        Particles[TrackID].Detectors[SensorID] = Detector{0, 0, G4ThreeVector(0, 0, 0)};
        return true;
    }
}

void ParticleInformation::SetHitAngle(G4int TrackID, G4int SensorID, G4double HitAngle)
{
    Particles[TrackID].Detectors[SensorID].HitAngle = HitAngle;
}

void ParticleInformation::SetHitPosition(G4int TrackID, G4int SensorID, G4ThreeVector HitPosition)
{
    Particles[TrackID].Detectors[SensorID].HitPosition = HitPosition;
}

void ParticleInformation::SetHitTime(G4int TrackID, G4int SensorID, G4double HitTime)
{
    Particles[TrackID].Detectors[SensorID].HitTime = HitTime;
}

void ParticleInformation::AddEnergyDeposit(G4int TrackID, G4int SensorID, G4double EnergyDeposit)
{
    Particles[TrackID].Detectors[SensorID].EnergyDeposit += EnergyDeposit;
}

void ParticleInformation::Parse()
{
    // parse all the information from Particles
    for (auto &pair : Particles)
    {
        G4cout << "Particle : " <<  particleTable->FindParticle(pair.second.Particle_PDG)->GetParticleName() << "(" << pair.second.Particle_PDG << ")" << "\t\t (Track ID : " << pair.second.TrackID << ")" << G4endl;
        G4cout << "---- E0 : " << pair.second.E0 << " keV" << G4endl;
        G4cout << "---- Dir : " << pair.second.Dir << G4endl;
        G4cout << "---- Pos : " << pair.second.Pos << G4endl;
        for (auto &pair2 : pair.second.Detectors)
        {
            G4cout << "---- Detector : " << pair2.first << G4endl;
            G4cout << "-------- Energy Deposit : " << pair.second.Detectors[pair2.first].EnergyDeposit << " keV" << G4endl;
            G4cout << "-------- Hit Angle : " << pair.second.Detectors[pair2.first].HitAngle << " deg" << G4endl;
            G4cout << "-------- Hit Position : " << pair.second.Detectors[pair2.first].HitPosition << G4endl;
        }
    }
}

void ParticleInformation::Clear()
{
    Particles.clear();
}


std::map<int, Particle> ParticleInformation::GetInfo()
{
    return Particles;
}

