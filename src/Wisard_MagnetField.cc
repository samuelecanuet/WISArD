#include "Wisard_MagnetField.hh"

WisardMagnetField::WisardMagnetField(G4double value)
{
//   ifstream file("MAGNETIC_FIELD_data/wisard_field.txt"); // Open the file for reading.

//   // Storage space for the table
//   // table dimensions
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

  // Ignore first blank line<
  char buffer[256];
  file.getline(buffer, 256);

  // Read in the data
 double z, Bz10, Br10, Bz28, Br28, Bz48, Br48, Bz68, Br68, Bz88, Br88;
  
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

    zval[iz] = z * mm;
  }
  file.close();

  FIELD_VALUE = value;

}

// void WisardMagnetField::GetFieldValue(const G4double point[4], G4double *Bfield) const
// {
//   // G4double x = point[0];
//   // G4double y = point[1];
//  G4double z = point[2];

//   // Check that the point is within the defined region
//   if (z < min)
//   {
//     Bfield[0] = 0.0;
//     Bfield[1] = 0.0;
//     Bfield[2] = 0.0;
//   }
//   else if (z >= min && z <= max)
//   {
//     // find two grid point around point searched for
//     for (int ix = 1; ix < nz; ix++)
//     {
//       if (zval[ix] > z)
//       {
//         Bfield[0] = 0.0;
//         Bfield[1] = 0.0;
//         Bfield[2] = (zField[ix] - zField[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + zField[ix - 1];
//         break;
//       }
//     }
//   }
//   else
//   {
//     Bfield[0] = 0.0;
//     Bfield[1] = 0.0;
//     Bfield[2] = 0.0;
//   }
// }


void WisardMagnetField::GetFieldValue(const G4double point[4], G4double *Bfield) const
{
  G4double x = point[0];
  G4double y = point[1];
  G4double r = sqrt(x * x + y * y);
  G4double z = point[2];

  // Check that the point is within the defined region
  if (z < min)
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = 0.0;
  }
  else if (z >= homogenous_min && z <= homogenous_max)
  {
    Bfield[0] = 0.0;
    Bfield[1] = 0.0;
    Bfield[2] = FIELD_VALUE;
  }
  else if (z >= min && z <= max)
  {
    // find two grid point around point searched for
    for (int ix = 1; ix < nz; ix++)
    {
      if (zval[ix] > z)
      {
        if (r <= 10)
        {
          Bfield[0] = (x)/r * ((rField10[ix] - rField10[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + rField10[ix - 1]);
          Bfield[1] = (y)/r * ((rField10[ix] - rField10[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + rField10[ix - 1]);
          Bfield[2] = (zField10[ix] - zField10[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + zField10[ix - 1];
          break;
        }

        else if (r <= 28)
        {
          Bfield[0] = (x)/r * ((rField28[ix] - rField28[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + rField28[ix - 1]);
          Bfield[1] = (y)/r * ((rField28[ix] - rField28[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + rField28[ix - 1]);
          Bfield[2] = (zField28[ix] - zField28[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + zField28[ix - 1];
          break;
        }
        else if (r <= 48)
        {
          Bfield[0] = (x)/r * ((rField48[ix] - rField48[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + rField48[ix - 1]);
          Bfield[1] = (y)/r * ((rField48[ix] - rField48[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + rField48[ix - 1]);
          Bfield[2] = (zField48[ix] - zField48[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + zField48[ix - 1];
          break;
        }
        else if (r <= 68)
        {
          Bfield[0] = (x)/r * ((rField68[ix] - rField68[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + rField68[ix - 1]);
          Bfield[1] = (y)/r * ((rField68[ix] - rField68[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + rField68[ix - 1]);
          Bfield[2] = (zField68[ix] - zField68[ix - 1]) / (zval[ix] - zval[ix - 1]) * (z - zval[ix - 1]) + zField68[ix - 1];
          break;
        }
        else if (r <= 88)
        {
          Bfield[0] = (x)/r * ((rField88[ix] - rField88[ix-1]) / (zval[ix]-zval[ix-1]) * (z-zval[ix-1]) + rField88[ix-1]);
          Bfield[1] = (y)/r * ((rField88[ix] - rField88[ix-1]) / (zval[ix]-zval[ix-1]) * (z-zval[ix-1]) + rField88[ix-1]);
          Bfield[2] = (zField88[ix] - zField88[ix-1]) / (zval[ix]-zval[ix-1]) * (z-zval[ix-1]) + zField88[ix-1];
          break;
        }
        else
        {
          Bfield[0] = 0.0;
          Bfield[1] = 0.0;
          Bfield[2] = 0.0;
        }
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
