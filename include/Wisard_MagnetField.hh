#include "globals.hh"
#include "Wisard_Global.hh"
#include "G4MagneticField.hh"
#include "G4ios.hh"

#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class WisardMagnetField
    : public G4MagneticField
// #endif
{

  int nz;
  G4double max, min;
  G4double xField[1001], yField[1001], zField[1001], zval[1001];

  double zField10[2534], rField10[2534];
  double zField28[2534], rField28[2534];
  double zField48[2534], rField48[2534];
  double zField68[2534], rField68[2534];
  double zField88[2534], rField88[2534];

  G4double FIELD_VALUE;
  G4double homogenous_min, homogenous_max;

public:
  WisardMagnetField(G4double value);

  void GetFieldValue(const G4double Point[3],
                     G4double *Bfield) const;
};
