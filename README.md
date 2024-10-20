# Scintillator Simulator - About
This repository contains the code for a LiquidO scintillator simulator project, completed as part of ENPH 455-Engineering Physics Thesis at Queen's University. The code simulates different liquid scintillator designs in a Monte Carlo format to provide metrics for theoretical LiquidO scintillator designs given user-tuned parameters.

## Setup and Dependancies
After cloning the repo to a local machine, the following dependencies are necessary to run the C++:
 - GNU Make (3.81+): Used for cleaning, building, and running C++ code
 - gcc (13.1.0+): C++ compiler called in the Makefile

And the following must be installed to run the accompanying python plotting script:
 - Python3 (3.11.9+)
 - pip install numpy
 - pip install matplotlib

## Usage
The C++ code is run directly from the 'src' dir via make, and brings up a CLI where the user may configure the scintillator properties before beginning the simualtion. Program outputs including photon paths, fiber positions, and scattering points for debugging are sent to the 'out' dir after simulation completion. To begin the C++ program, navigate to the 'src' dir and run: "make build" -> "make run".

The python plotting script is within the 'plotting' dir, and scrapes information from the generated 'out' dir text files. This script may be run directly from within the C++ CLI, or command-line after exiting the C++ interface. The default behaviour of the script is to save static plots to the 'out' dir, however, adjusting the file path's and running this script in an editor that supports %matplotlib auto (such as Spyder or JupyterNotebook) will create active plots.

- Usage: python ./ShowScattering.py -func {plot2d,plot2d_collsions,plot3d,plot_wavelength_hist} -sc_type {LAB,DIN} [-fiber_diam DIAM] [-d]
   - Required:
      - func : Plot type to be generated
      - sc_type : LiquidO scintillator cocktail solution
   - Optional
      - fiber_diam : Diameter of simualted fibers. If nothing provided, plots will not include fibers
      - d : Flag for enabling debugging points at scattering positions (supported for plot2d)

- Example: python ./ShowScattering.py -func plot2d_collsions -sc_type LAB -fiber_diam 0.5
   - Generates a stacked plot showing all end positions of photons as well as a heatmap of fiber collisions in a 2d (x,y) plane. Scattering origin also shown.
   - ![fiberCollisions](https://github.com/user-attachments/assets/1462b79b-bc7c-4fb6-a639-7a8ecde999a6)




