# WISArD Geant4 Simulation

This code is a Geant4 simulation for the WISArD experiment at ISOLDE CERN.

## Prerequisites
- **Geant4 11.1.2**
- **CLHELP**
- **ROOT** 
- **PyROOT** (only for *Analyze.py*)
- **Environement variables :**
```bash
source $G4INSTALL/share/Geant4/geant4make/geant4make.csh $G4INSTALL/share/Geant4/geant4make/
setenv PATH ${PATH}:$WhereGitIs/WISArD/bin/Linux-g++
```

- Only 8B, 8Li, 14O, 20Mg, 24Si, 26S and 32Ar at 30 keV can be use in the simulation because there is a SRIM file for them. But you can generate a file for an other nucleus or energy with SRIM. You have to rename a *RANGE_3D.txt* SRIM file and place it in the SRIM_data folder.

## Usage
### To run the simulation in visualisation mode :
```bash
wisard 
```
### To run the simulation in test mode :
```bash
wisard macro.mac
```

### To generate the CRADLE file and run the Geant4 simulation :
*Exemple for 32Ar with 10 threads in Standard Model case*

```bash
CRADLE-G4 nucleus=32Ar N=10 a=1 b=1
```
*or*
```bash
CRADLE-G4 nucleus=32Ar N=10 CS=0 CV=1 events=1000
```
If one Standard Model parameter is not given it will be set in SM case. Additionaly, *macro_base.mac* will be use as macro sample.
See --help option to see all the options.

## Result
You end up a Tree, detectors histograms and simulation paramater in the ROOT TFile generated.

### Simulation informations
Located at the beginning of the TFile, you can find all the simulation variables chosen.

### Tree
The Tree is update each 10 000 events and contains :
- Event Number
- Gun Information (particle PDG, position, direction, energy, time)
- Catchers Energy deposit
- Plastic Scintillator (deposit energy, hit position, hit angle, hit time)
- Silicon Detectors (deposit energy, hit position, hit angle, hit time, detector code)

### Histograms
There are 2 for each strip, one for the β-p coincidence and an other one for the anti-coincidence. The β detection threashold is set in the macro file.

## Analysis
A very rapid analysis was made with PyROOT. To use it you have to indicate the filename in the main function.


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

### v2.4
- Adding 2021 set-up version (indicated in the *macro* file)
- Discard all the inter-strip event in histograms

### v2.5
- Simplification of the simulation (deleting particle information of the energy deposit)
- New TTree 
- Output file include coincidence Plastic Scintillator deposit energy

### v3.0
- Re-implemenatation of particle independant information saving
- Adding Al grid on silicon detectors
- Adding interstrip saving information
- Modification of distance in geometry

### v4.0
- Decay physics implemented to use radioactive nucleus in the particle gun
- Modification of particle information saving for parent-secondaries (*ParticleInformation.xx*)
- New messenger for particle gun
- macro modification
- In nucleus decay particule gun all particle and nucleus are saved in the tree

### v4.1
- Personnalized messenger for all the classes
- Adding Killer volume in the geometry for computing time performance *Wisard_Killer*
- Adding particle and ion gun
- Adding MaxNumberOfStep = 100000 to avoid infinit simulation
- Adding StepMaxLength to Silicon Deadlayer, Grid and catcher Al complementation side

### v4.2
- Geometry modification for strips
- Using ROOT file as input for SRIM implementation
- Removing stepping information saved in interstrip (implemented in v3.0)
- Adding IC for 207Bi decay in PhysicList::ConstructParticle
- Adding the collimator in geometry

### v4.3
- MT implementation
- Collimator option in the macro
- Geometry of 2025 added (beamline + collimator)
- Full Magnetic field implementation as option in the macro