#include "Wisard_MagnetField.hh"

WisardMagnetField::WisardMagnetField(G4double value)
{
  // ifstream file("MAGNETIC_FIELD_data/wisard_field.txt"); // Open the file for reading.

  // Storage space for the table
  // table dimensions
//   nz = 1001;
//   max = 50. * cm, min = -50. * cm;

//   // Ignore first blank line
//   char buffer[256];
//   file.getline(buffer, 256);

//   // Read in the data
//  G4double z, bx, by, bz;
//   for (int iz = 0; iz < nz; iz++)
//   {
//     file >> z >> bz >> by >> bx;
//     zField[iz] = bz / 6 * value;
//     yField[iz] = by * tesla;
//     xField[iz] = bx * tesla;
//     zval[iz] = z * meter;
//   }
//   file.close();

  ifstream file("MAGNETIC_FIELD_data/wisard_field_complete.txt"); // Open the file for reading.

  // Storage space for the table
  // table dimensions
  nz = 2534;
  max = 50. * cm, min = -1. * m;
  homogenous_max = 20*cm, homogenous_min = -20*cm;

  // Ignore first blank line
  char buffer[256];
  file.getline(buffer, 256);

  // Read in the data
  double z, Bz10, Br10, Bz28, Br28, Bz48, Br48, Bz68, Br68, Bz88, Br88;

  graphBz = new TGraph2D();
  graphBr = new TGraph2D();
  
  for (int iz = 0; iz < nz; iz++)
  {
    file >> z >> Bz10 >> Br10 >> Bz28 >> Br28 >> Bz48 >> Br48 >> Bz68 >> Br68 >> Bz88 >> Br88;
    zField10[iz] = Bz10 * tesla;
    rField10[iz] = Br10 * tesla;
    zField28[iz] = Bz28 * tesla;
    rField28[iz] = Br28 * tesla;
    zField48[iz] = Bz48 * tesla;
    rField48[iz] = Br48 * tesla;
    zField68[iz] = Bz68 * tesla;
    rField68[iz] = Br68 * tesla;
    zField88[iz] = Bz88 * tesla;
    rField88[iz] = Br88 * tesla;

    graphBz->AddPoint(z*mm, 0*mm , Bz10 / 4 * value);
    graphBr->AddPoint(z*mm, 0*mm , 0 / 4 * value);
    graphBz->AddPoint(z*mm, 10*mm , Bz10 / 4 * value);
    graphBr->AddPoint(z*mm, 10*mm , Br10 / 4 * value);
    graphBz->AddPoint(z*mm, 28*mm , Bz28 / 4 * value);
    graphBr->AddPoint(z*mm, 28*mm , Br28 / 4 * value);
    graphBz->AddPoint(z*mm, 48*mm , Bz48 / 4 * value);
    graphBr->AddPoint(z*mm, 48*mm , Br48 / 4 * value);
    graphBz->AddPoint(z*mm, 68*mm , Bz68 / 4 * value);
    graphBr->AddPoint(z*mm, 68*mm , Br68 / 4 * value);
    graphBz->AddPoint(z*mm, 88*mm , Bz88 / 4 * value);
    graphBr->AddPoint(z*mm, 88*mm , Br88 / 4 * value);    

    zval[iz] = z * mm;
  }
  file.close();

  FIELD_VALUE = value;

}

/*
void WisardMagnetField::GetFieldValue(const G4double point[4], G4double *Bfield) const
{
  // G4double x = point[0];
  // G4double y = point[1];
 G4double z = point[2];

  // Check that the point is within the defined region
  if (z < min)
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
  else if (z <= max)
  {
    // find two grid point around point searched for
    for (int ix = 1; ix < nz; ix++)
    {
      if (zval[ix] > z)
      {
        Bfield[0] = 0.0;
        Bfield[1] = 0.0;
        Bfield[2] = (zField[ix] - zField[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + zField[ix - 1];
        break;
      }
    }
  }
  else
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
}

*/
void WisardMagnetField::GetFieldValue(const G4double point[4], G4double *Bfield) const
{
  G4double x = point[0];
  G4double y = point[1];
  G4double r = sqrt(x * x + y * y);
  G4double z = point[2];

  if (z < min)
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
  else if (z >= min && z <= max)
  {
    double Br = graphBr->Interpolate(z * mm, r * mm);
    double Bz = graphBz->Interpolate(z * mm, r * mm);

    if (r != 0)
    {
      // Normalize the radial field component
      Br /= r;
    }
    else
    {
      Br = 0.0; // Avoid division by zero
    }

    Bfield[0] = (x) * Br;
    Bfield[1] = (y) * Br;
    Bfield[2] = Bz;
  }
  else
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
}
