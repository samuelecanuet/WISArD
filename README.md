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

- Only 8B, 8Li, 14O, 20Mg, 24Si, 26S and 32Ar at 30 keV can be use in the simulation because there is a SRIM file for them. But you can generate a file for a other nucleus or energy with SRIM. You have to rename a *RANGE_3D.txt* SRIM file and place it in the SRIM_data folder.

## Usage
### To run the simulation in visualisation mode :
```bash
wisard 
```
### To run the simulation data mode :
```bash
wisard macro.mac
```

### To run the simulation multithreading mode :
*Exemple for 32Ar with 10 runs and 2 threads in Standard Model case*

```bash
Multith_CRADLE-G4 nucleus=32Ar N=10 n=2 a=1 b=1
```
*or*
```bash
Multith_CRADLE-G4 nucleus=32Ar N=10 n=2 CS=0 CV=1
```
If one parameter is not given it will be set in SM case. Additionaly, *macro_base.txt* will be use as macro sample.

In this mode you can run CRADLE++ in the same time to generate file for the Geant4 simulation. But if you want to use your own files, you have just to comment the *line 214* in *Multith_CRADLE-G4*. Your file have to be generate by CRADLE++ or with the same architecture.
You have to take into account that the file are written like :
```bash
32Ar_a1_b0_x.txt or .root
```
*or*
```bash
32Ar_CS=0_CV=1_x.txt or .root
```
With x the number of the run (x<=N).

## Result
You end up (N ROOT files if you are un MultiThreading mode) with simulation informations, a Tree and a histogram of energy deposit for each detector in a ROOT file. The filename is set in the macro file.

### Simulation informations
Located in the *Log* folder with all the simulation variables chosen in the macro file.

### Tree
The Tree is update each 10 000 events and contains :
- Event Number
- Gun Information (particle PDG, position, direction, energy)
- Catchers Energy deposit
- Plastic Scintillator (deposit energy, hit position, hit angle)
- Silicon Detectors (deposit energy, hit position, hit angle, detector code, deposit energy in dead layer)

### Histograms
There are 2 for each strip, one for the β-p coincidence and an other one the none-coincidence. The β detection threashold is set in the macro file.

## Analysis
A very rapid analysis is with PyROOT. To use it you have to indicate the filename in the main function.


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