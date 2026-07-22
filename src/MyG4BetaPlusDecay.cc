#include "MyG4BetaPlusDecay.hh"
#include "G4BetaDecayCorrections.hh"
#include "G4IonTable.hh"
#include "G4ThreeVector.hh"
#include "G4DynamicParticle.hh"
#include "G4DecayProducts.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <iomanip>

// #include "MyBetaSpectrumGenerator.hh"

#include "G4IsotopeProperty.hh"
#include "G4NuclideTable.hh"
#include "G4DecayTable.hh"

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/legendre.hpp>

using namespace std;

MyG4BetaPlusDecay::MyG4BetaPlusDecay(const G4ParticleDefinition *theParentNucleus,
                                     const G4double &branch, const G4double &e0,
                                     const G4double &excitationE,
                                     const G4Ions::G4FloatLevelBase &flb,
                                     const G4BetaDecayType &betaType
                                    //  ,MyG4RadioactiveDecay *theDecay
                                    )
    : G4BetaPlusDecay(theParentNucleus, branch, e0, excitationE, flb, betaType),
      endpointEnergy(e0 - 2. * CLHEP::electron_mass_c2),
      ExcitationEnergy(excitationE),
      FLB(flb)
{
  BETATYPE=betaType;
  G4IonTable *theIonTable =
      (G4IonTable *)(G4ParticleTable::GetParticleTable()->GetIonTable());
  daughterZ = theParentNucleus->GetAtomicNumber() - 1;
  daughterA = theParentNucleus->GetAtomicMass();
  // G4DecayTable *t = theDecay->GetDecayTable((theIonTable->GetIon(daughterZ, daughterA, excitationE, flb)));

  
  endpointEnergy_New = endpointEnergy;
  SetUpBetaSpectrumSampler(daughterZ, daughterA, BETATYPE);

  SetParent(theParentNucleus); // Store name of parent nucleus, delete G4MT_parent
  SetBR(branch);

  SetNumberOfDaughters(3);
  SetDaughter(0, theIonTable->GetIon(daughterZ, daughterA, excitationE, flb));
  
  SetDaughter(1, "e+");
  SetDaughter(2, "nu_e");

  // betaSpectrumGenerator = new MyBetaSpectrumGenerator(theParentNucleus, 1, endpointEnergy, 0, 1);
  // betaSpectrumGenerator->Generate();
}

MyG4BetaPlusDecay::~MyG4BetaPlusDecay() {}

G4DecayProducts* MyG4BetaPlusDecay::DecayIt(G4double)
{
  // G4cout << "\033[1;32m" << "MyG4BetaPlusDecay::DecayIt" << "\033[0m" << G4endl;
  // G4IsotopeProperty *p = G4NuclideTable::GetInstance()->GetIsotope(G4MT_daughters[0]->GetAtomicNumber(), G4MT_daughters[0]->GetAtomicMass(), ExcitationEnergy);
  // EnergyBroadering = 0.0;
  // if (p)
  //   EnergyBroadering = ConvertingHalflifeToWidth(G4NuclideTable::GetInstance()->GetIsotope(G4MT_daughters[0]->GetAtomicNumber(), G4MT_daughters[0]->GetAtomicMass(), ExcitationEnergy)->GetLifeTime()) * MeV;

  // Fill G4MT_parent with theParentNucleus (stored by SetParent in ctor)
  CheckAndFillParent();

  // Fill G4MT_daughters with e-, nu and residual nucleus (stored by SetDaughter)
  CheckAndFillDaughters();

  
  // Set up final state
  // parentParticle is set at rest here because boost with correct momentum
  // is done later
  G4DynamicParticle parentParticle(G4MT_parent, G4ThreeVector(0, 0, 0), 0.0);
  G4DecayProducts *products = new G4DecayProducts(parentParticle);
  // std::vector<G4double> VectorExcitationEnergy;

  // G4double Delta_daughterExcitationEnergy = ShootBreitWigner(EnergyBroadering);
  
  endpointEnergy_New = endpointEnergy;// + Delta_parentExcitationEnergy - Delta_daughterExcitationEnergy;
  
  G4double parentMass = G4MT_parent->GetPDGMass();
  G4double eMass = G4MT_daughters[1]->GetPDGMass();
  G4double nucleusMass = G4MT_daughters[0]->GetPDGMass();

  if (spectrumSampler)
  {
    // Generate positron isotropic in angle, with energy from stored spectrum
    G4double eKE = (endpointEnergy_New)*spectrumSampler->shoot(G4Random::getTheEngine());
    // G4double eKE = betaSpectrumGenerator->Shoot().first;
    // G4double change = betaSpectrumGenerator->Shoot().second;
    G4double eMomentum = std::sqrt(eKE * (eKE + 2. * eMass));

    G4double cosTheta = 2. * G4UniformRand() - 1.0;
    G4double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    G4double phi = twopi * G4UniformRand() * rad;
    G4double sinPhi = std::sin(phi);
    G4double cosPhi = std::cos(phi);

    G4ParticleMomentum eDirection(sinTheta * cosPhi, sinTheta * sinPhi, cosTheta);
    G4DynamicParticle *dynamicPositron = new G4DynamicParticle(G4MT_daughters[1], eDirection * eMomentum);
    products->PushProducts(dynamicPositron);
    // VectorExcitationEnergy.push_back(0.0);

    // Generate neutrino with angle relative to positron, and energy from
    // energy-momentum conservation using endpoint energy of reaction
    G4double eTE = eMass + eKE;
    G4double cosThetaENu = GetAngle(eMomentum / (eTE));

    G4double nuEnergy = ((endpointEnergy_New - eKE) * (parentMass + nucleusMass - eTE) - eMomentum * eMomentum) / (parentMass - eTE + eMomentum * cosThetaENu) / 2.;

    G4double sinThetaENu = std::sqrt(1.0 - cosThetaENu * cosThetaENu);
    phi = twopi * G4UniformRand() * rad;
    G4double sinPhiNu = std::sin(phi);
    G4double cosPhiNu = std::cos(phi);

    G4ParticleMomentum nuDirection;
    nuDirection.setX(sinThetaENu * cosPhiNu * cosTheta * cosPhi -
                     sinThetaENu * sinPhiNu * sinPhi + cosThetaENu * sinTheta * cosPhi);
    nuDirection.setY(sinThetaENu * cosPhiNu * cosTheta * sinPhi +
                     sinThetaENu * sinPhiNu * cosPhi + cosThetaENu * sinTheta * sinPhi);
    nuDirection.setZ(-sinThetaENu * cosPhiNu * sinTheta + cosThetaENu * cosTheta);

    G4DynamicParticle *dynamicNeutrino = new G4DynamicParticle(G4MT_daughters[2], nuDirection * nuEnergy);
    products->PushProducts(dynamicNeutrino);
    // VectorExcitationEnergy.push_back(0.0);

    // Generate daughter nucleus from sum of positron and neutrino 4-vectors:
    // p_D = - p_e - p_nu
    G4DynamicParticle *dynamicDaughter =
        new G4DynamicParticle(G4MT_daughters[0],
                              -eDirection * eMomentum - nuDirection * nuEnergy);
    products->PushProducts(dynamicDaughter);
    // VectorExcitationEnergy.push_back(Delta_daughterExcitationEnergy);
  }
  else
  {
    // positron energy below threshold -> no decay
    G4DynamicParticle *noDecay =
        new G4DynamicParticle(G4MT_parent, G4ThreeVector(0, 0, 0), 0.0);
    products->PushProducts(noDecay);
  }

  // Check energy conservation against endpoint value, not nuclear masses
  G4int nProd = products->entries();
  G4DynamicParticle *temp = 0;
  G4double Esum = 0.0;
  for (G4int i = 0; i < nProd; i++)
  {
    temp = products->operator[](i);
    Esum += temp->GetKineticEnergy();
  }
  G4double eCons = (endpointEnergy_New - Esum) / keV;
  if (eCons > 0.001)
    G4cout << " Beta+ check: eCons (keV) = " << eCons << G4endl;

  // return make_pair(products, VectorExcitationEnergy);
  return products;
}

void MyG4BetaPlusDecay::SetUpBetaSpectrumSampler(const G4int &daughterZ,
                                                 const G4int &daughterA,
                                                 const G4BetaDecayType &betaType)
{
  G4double e0 = endpointEnergy_New / CLHEP::electron_mass_c2;
  G4BetaDecayCorrections corrections(-daughterZ, daughterA);
  spectrumSampler = 0;

  // Check for cases in which Q < 2Me (e.g. z67.a162)
  if (e0 > 0.)
  {
    // Array to store spectrum pdf
    G4int npti = 100;
    G4double *pdf = new G4double[npti];

    G4double e; // Total positron energy in units of electron mass
    G4double p; // Positron momentum in units of electron mass
    G4double f; // Spectral shap function
    for (G4int ptn = 0; ptn < npti; ptn++)
    {
      // Calculate simple phase space
      e = 1. + e0 * (ptn + 0.5) / G4double(npti);
      p = std::sqrt(e * e - 1.);
      f = p * e * (e0 - e + 1.) * (e0 - e + 1.);

      // Apply Fermi factor to get allowed shape
      f *= corrections.FermiFunction(e);

      // Apply shape factor for forbidden transitions
      f *= corrections.ShapeFactor(betaType, p, e0 - e + 1.);
      pdf[ptn] = f;
    }
    spectrumSampler = new G4RandGeneral(pdf, npti);
    delete[] pdf;
  }
}

double MyG4BetaPlusDecay::GetAngle(double e)
{

  std::vector<std::vector<double>> dist;
  int N = 10000;
  double stepSize = 1. / N;
  double currentCosAngle = -1;

  while (currentCosAngle <= 1)
  {
    std::vector<double> pair;
    pair.push_back(currentCosAngle);
    double W = 1 + a * e * currentCosAngle;
    pair.push_back(W);
    dist.push_back(pair);
    currentCosAngle += stepSize;
  }

  double costheta = RandomFromDistribution(dist);
  return costheta;
}

G4double MyG4BetaPlusDecay::RandomFromDistribution(vector<vector<double>> pd)
{
  double begin = pd[0][0];
  double end = pd[pd.size() - 1][0];
  double max = 0.;

  for (std::vector<std::vector<double>>::size_type i = 0; i != pd.size(); i++)
  {
    if (pd[i][1] > max)
    {
      max = pd[i][1];
    }
  }

  double r = 0.;
  double q = 0.;
  do
  {
    r = rand() / (double)RAND_MAX;
    q = rand() / (double)RAND_MAX * max;
  } while (q > pd[r * pd.size()][1]);
  return r * (end - begin) + begin;
}

G4double MyG4BetaPlusDecay::ConvertingHalflifeToWidth(G4double halflife)
{
  // G4cout << log(2) * CLHEP::hbar_Planck/(halflife*s) << G4endl;
  return log(2) * CLHEP::hbar_Planck / (halflife * ns);
}

G4double MyG4BetaPlusDecay::ShootBreitWigner(G4double gamma)
{
  double a = gamma / 2;
  double r = rand() / (double)RAND_MAX;
  double x = a * tan(M_PI * (r - 0.5));
  return x;
}
