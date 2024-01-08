# WISArD Geant4 Simulation

This code is a Geant4 simulation for the WISArD experiment at ISOLDE CERN.

## Prerequisites
- **Geant4 11.1.2**
- **ROOT** 
- **PyROOT** (only for *Analyze.py*)
- **Environement variables :**
```bash
source $G4INSTALL/geant4make.csh $G4INSTALL
setenv PATH ${PATH}:$WhereGitIs/WISArD/bin/Linux-g++
```

## Usage
:warning: 


### To run the simulation in visualisation mode :

```bash
wisard 
```

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

### v2.3
- Add Garage (more precise position as to be done)
- More accurate particle recording information in sensor
- Add ROOT from CRADLE as input file possible

### TODO
- Add Source in the Catcher (+adapting the generator)


(v2.1 1e6 3h)
(v2.2 1e6 1h)
(v2.3 1e6 35min)