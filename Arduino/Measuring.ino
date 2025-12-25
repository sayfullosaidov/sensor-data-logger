/*
 Example using two SparkFun HX711 breakout boards with two separate scales
 By: Your Name (modified from Nathan Seidle's example)
 Date: July 2, 2025
 License: Public Domain / Beerware

 This example demonstrates reading from two independent load cells simultaneously.
 Each load cell requires its own HX711 amplifier and a dedicated pair of Arduino pins.

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
*/

#include "HX711.h"

// Define calibration factors for each scale
// YOU WILL NEED TO CALIBRATE EACH SCALE INDIVIDUALLY AND REPLACE THESE VALUES
#define CALIBRATION_FACTOR_SCALE_A -13453.4257 // Example value for Scale A
#define CALIBRATION_FACTOR_SCALE_B -471475.0000 // Example value for Scale B

// Pin definitions for Scale A
#define DOUT_A  3
#define CLK_A   2

// Pin definitions for Scale B
#define DOUT_B  5
#define CLK_B   4

// Create two HX711 objects, one for each scale
HX711 scaleA;
HX711 scaleB;

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 Dual Scale Demo");

  // Initialize Scale A
  Serial.println("Initializing Scale A...");
  scaleA.begin(DOUT_A, CLK_A);
  scaleA.set_scale(CALIBRATION_FACTOR_SCALE_A);
  scaleA.tare(); // Tare Scale A
  Serial.println("Scale A Ready.");

  // Initialize Scale B
  Serial.println("Initializing Scale B...");
  scaleB.begin(DOUT_B, CLK_B);
  scaleB.set_scale(CALIBRATION_FACTOR_SCALE_B);
  scaleB.tare(); // Tare Scale B
  Serial.println("Scale B Ready.");

  Serial.println("\nReadings:");
}

void loop() {

  // Read from Scale A
  Serial.print("A (g): ");
  Serial.print(scaleA.get_units(), 1); // scaleA.get_units() returns a float

  // Read from Scale B
  Serial.print(" B (g): ");
  Serial.print(scaleB.get_units(), 1); // scaleB.get_units() returns a float
  Serial.println();


  delay(500); // 1000 milliseconds = 1 second

}