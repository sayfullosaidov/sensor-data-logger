/*
 Example using two SparkFun HX711 breakout boards for calibration
 Modified by: Your Name (based on Nathan Seidle's calibration sketch)
 Date: July 2, 2025
 License: Public Domain / Beerware

 This sketch allows you to calibrate two independent HX711 load cell setups
 by entering an exact known weight value. You will calibrate each scale individually.

 IMPORTANT:
 - Remove all weight from both scales before starting the sketch.
 - After readings begin, select the scale to calibrate (e.g., type '1' for Scale A).
 - Follow the prompts to place a KNOWN WEIGHT on the selected scale and enter its exact value.
 - Once the readings match your known weight, note down the calibration factor for that scale.
 - You can still use '+' or '-' for fine-tuning after an exact value is entered.
 - Repeat the process for the other scale.

 in Serial.print statements and adjust your known weights accordingly.

 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
*/

#include "HX711.h"

// Pin definitions for Scale A
#define DOUT_A  3
#define CLK_A   2

// Pin definitions for Scale B
#define DOUT_B  5
#define CLK_B   4 // Ensure these pins are different from DOUT_A/CLK_A

// Create two HX711 objects
HX711 scaleA;
HX711 scaleB;

// Initial calibration factors (these will be adjusted during calibration)
// Start with a rough estimate or the value from your single scale test.
// These values will be overwritten when you enter a known weight.
float calibration_factor_A = -7050.0; // Starting point for Scale A
float calibration_factor_B = -7050.0; // Starting point for Scale B

// Variables to manage calibration state
int activeScale = 0; // 0 for none, 1 for Scale A, 2 for Scale B
bool waitingForWeightInput = false; // Flag to indicate if we are waiting for a numerical weight input

void setup() {
  Serial.begin(9600);
  Serial.println("--- HX711 Dual Scale Calibration Sketch (Exact Value Input) ---");
  Serial.println("Remove all weight from BOTH scales.");

  // Initialize Scale A
  Serial.println("\nInitializing Scale A...");
  scaleA.begin(DOUT_A, CLK_A);
  scaleA.set_scale(); // Set to default (no calibration factor applied yet for tare)
  scaleA.tare();      // Reset Scale A to 0 (reads a baseline raw value)
  long zero_factor_A = scaleA.read_average(); // Get a baseline reading for reference
  Serial.print("Scale A Zero factor (raw average): ");
  Serial.println(zero_factor_A);
  Serial.println("Scale A Ready.\n");

  // Initialize Scale B
  Serial.println("Initializing Scale B...");
  scaleB.begin(DOUT_B, CLK_B);
  scaleB.set_scale(); // Set to default
  scaleB.tare();      // Reset Scale B to 0
  long zero_factor_B = scaleB.read_average();
  Serial.print("Scale B Zero factor (raw average): ");
  Serial.println(zero_factor_B);
  Serial.println("Scale B Ready.\n");

  printInstructions(); // Display initial instructions
}

// Function to print main instructions
void printInstructions() {
  Serial.println("\n----------------------------------------------");
  Serial.println("To calibrate Scale A, type '1' and press Enter.");
  Serial.println("To calibrate Scale B, type '2' and press Enter.");
  Serial.println("----------------------------------------------");
}

void loop() {
  // Handle serial input
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read the entire line until newline
    input.trim(); // Remove any leading/trailing whitespace

    if (waitingForWeightInput) {
      // If we are currently waiting for a numerical weight input
      float knownWeight = input.toFloat(); // Convert the input string to a float

      // Check if the conversion was successful and the weight is not zero
      if (knownWeight != 0.0) {
        long rawReading;
        if (activeScale == 1) {
          rawReading = scaleA.get_value(); // Get a raw ADC reading from Scale A
          if (rawReading != 0) { // Avoid division by zero if raw reading is unexpectedly zero
            // Calculate new calibration factor: (raw reading after tare) / known weight
            calibration_factor_A = (float)rawReading / knownWeight;
            Serial.print("New Cal Factor A calculated: ");
            Serial.println(calibration_factor_A, 4); // Print with 4 decimal places for precision
          } else {
            Serial.println("Error: Raw reading from Scale A is zero. Cannot calculate calibration factor.");
          }
        } else if (activeScale == 2) {
          rawReading = scaleB.get_value(); // Get a raw ADC reading from Scale B
          if (rawReading != 0) { // Avoid division by zero
            calibration_factor_B = (float)rawReading / knownWeight;
            Serial.print("New Cal Factor B calculated: ");
            Serial.println(calibration_factor_B, 4);
          } else {
            Serial.println("Error: Raw reading from Scale B is zero. Cannot calculate calibration factor.");
          }
        }
        waitingForWeightInput = false; // Reset the flag as we've received the weight
        Serial.println("Calibration updated. You can now fine-tune with '+' or '-' or enter a new known weight.");
        // We remain in the activeScale state to allow continuous monitoring or further refinement.
      } else {
        Serial.println("Invalid weight entered. Please enter a numerical value (e.g., 10.5).");
      }
    } else {
      // If we are not waiting for a numerical weight input, check for commands
      char command = input.charAt(0); // Get the first character of the input

      if (command == '1') {
        activeScale = 1;
        waitingForWeightInput = true; // Set flag to expect a numerical input next
        Serial.println("\n--- Calibrating Scale A ---");
        Serial.println("1. Place KNOWN WEIGHT on Scale A.");
        Serial.println("2. Type the exact weight (e.g., 10.5) and press Enter.");
        Serial.println("   (After entering, you can still use '+' or '-' for fine-tuning the factor)");
      } else if (command == '2') {
        activeScale = 2;
        waitingForWeightInput = true; // Set flag to expect a numerical input next
        Serial.println("\n--- Calibrating Scale B ---");
        Serial.println("1. Place KNOWN WEIGHT on Scale B.");
        Serial.println("2. Type the exact weight (e.g., 10.5) and press Enter.");
        Serial.println("   (After entering, you can still use '+' or '-' for fine-tuning the factor)");
      } else if (activeScale != 0) { // Only process '+' or '-' if a scale is already active
        if (command == '+' || command == 'a') {
          if (activeScale == 1) calibration_factor_A += 10;
          else if (activeScale == 2) calibration_factor_B += 10;
          Serial.println("Calibration factor adjusted incrementally.");
        } else if (command == '-' || command == 'z') {
          if (activeScale == 1) calibration_factor_A -= 10;
          else if (activeScale == 2) calibration_factor_B -= 10;
          Serial.println("Calibration factor adjusted incrementally.");
        } else {
          Serial.println("Unknown command. Please use '1', '2', '+', or '-'.");
        }
      } else {
        Serial.println("Please select a scale to calibrate first (type '1' or '2').");
      }
    }
  }

  // Display readings and current calibration factor for the active scale
  // If no scale is active, show readings for both with their current factors.
  if (activeScale == 1) {
    scaleA.set_scale(calibration_factor_A); // Apply the current calibration factor
    Serial.print("Scale A Reading: ");
    Serial.print(scaleA.get_units(), 1); // Display weight with 1 decimal place
    Serial.print(" g | Cal Factor A: ");
    Serial.print(calibration_factor_A, 4); // Display factor with 4 decimal places
    Serial.println();
  } else if (activeScale == 2) {
    scaleB.set_scale(calibration_factor_B); // Apply the current calibration factor
    Serial.print("Scale B Reading: ");
    Serial.print(scaleB.get_units(), 1);
    Serial.print(" g | Cal Factor B: ");
    Serial.print(calibration_factor_B, 4);
    Serial.println();
  } else {
    // When no scale is actively being calibrated, display readings for both
    scaleA.set_scale(calibration_factor_A);
    scaleB.set_scale(calibration_factor_B);
    Serial.print("Scale A (Current): "); Serial.print(scaleA.get_units(), 1); Serial.print(" g | ");
    Serial.print("Scale B (Current): "); Serial.print(scaleB.get_units(), 1); Serial.print(" g. ");
    Serial.println("Select a scale (1 or 2) to calibrate.");
  }
  delay(100); // Small delay to make serial output readable
}
