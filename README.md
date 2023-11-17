# WISArD Geant4 Simulation

This code is a Geant4 simulation for the WISArD experiment at ISOLDE CERN.

## Version History

### v1.0
- Basic WISArD code with the new geometry.

### v2.0
- Added a Messenger to interact more easily with some systematic variables.
- Developing different trees for each emitted particle.
- Moved the analysis from DeltaKineticEnergy to TotalEnergyDeposit.

### v2.1
- More accurate implantation point in the catcher with a 3D Histogram of coordinates from a RANGE_3D SRIM file.
- Considered all the random selections with the Geant4 seed.
- Writing logs to save the simulation parameters.
- Single Tree with new c++/python analysis

### v2.2
- 2023 Catcher Support Version
- Added Catcher position option in macro
- Replace String particle name by PDG code
- Replace String name detector to code
- Uniform Magnetic Field for optimisation

### TODO
- Add Garage
- Add Source in the Catcher 
- Change research between primary and detected in Rubmanager or Generator