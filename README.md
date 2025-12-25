# Sensor Data Logger & Buoyancy Model

## Overview
This repository contains two projects I worked on during my internship at Max Planck Institute for Dynamics and Self-Organization:

1. **Sensor Data Logger**: Arduino-based dual-load cell system using HX711 amplifiers. Data was logged with CoolTerm and analyzed in Python.
2. **Buoyancy Python Model**: A Python model to calculate helium volume required for a balloon to reach a certain altitude, accounting for pressure and temperature.

## Hardware & Tools
- Arduino Uno
- HX711 Load Cell Amplifiers
- SparkFun Load Cells
- CoolTerm for serial data logging

## Software
- Python 3 (Google Colab)
- Libraries: NumPy, Pandas, Matplotlib
- Arduino IDE

## Arduino Codes
- `dual_scale_reading.ino` → reads two scales simultaneously
- `calibration.ino` → calibration script for the load cells

## Python Notebooks
- `buoyancy_model.ipynb` → calculates helium volume required for altitude targets

## References
- [SparkFun HX711 Hookup Guide](https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide/all)
- [HX711 Arduino Library](https://github.com/bogde/HX711)

## CAD Projects
- Tube design in Onshape
  - Exported as STEP file
