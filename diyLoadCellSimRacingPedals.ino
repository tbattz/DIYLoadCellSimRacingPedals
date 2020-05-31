#if 1
__asm volatile ("nop");
#endif

#include "HX711.h"
#include "Joystick.h"

#define DOUT 3
#define CLK 2
#define DOUT2 6
#define CLK2 7

// Setup HX711 boards
HX711 throttleScale;
HX711 brakeScale;

// Create raw variables
float rawThrottle = 0;
float rawBrake = 0;
// Create conversion variables
// Line joining (a,0) to (b, 1023) with gradient m=1023/(b-a) and c=-ma
const float minRawThrottle = 10;
const float maxRawThrottle = 1200000;
const float mThrottle = 1023/(maxRawThrottle - minRawThrottle);
const float cThrottle = - mThrottle * minRawThrottle;
const float minRawBrake = 0;
const float maxRawBrake = 800000;
const float mBrake = 1023/(maxRawBrake - minRawBrake);
const float cBrake = - mBrake * minRawBrake;
// Create output variables
float adjThrottle = 0;
float adjBrake = 0;

// Create Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID + 1, // Don't use first joystick ID, as this is taken by the steering wheel device.
  JOYSTICK_TYPE_MULTI_AXIS, 0, 0,             // Button Counts
  false, false, false, false, false, false,   // Axes Includes
  false, true, false, true, false);           // Rudder, throttle, accelerator, brake, steering

// Set auto send mode on joystick
const bool testAutoSendMode = true;

// Plotting - Comment line(s) to disable plotting and/or timer counter
//#define PLOT;
//#define TIMER_COUNTER;

// Loop Timer
long lastMillis = 0;
long loops = 0;



void setup() {
  // Begin connection
  #ifdef PLOT || TIMER_COUNTER
    Serial.begin(9600);
  #endif

  // Setup first HX711
  throttleScale.begin(DOUT, CLK);
  throttleScale.set_scale();
  throttleScale.tare(); // Set scale to 0
  long zero_factor = throttleScale.read_average();
  
  // Setup second HX711
  brakeScale.begin(DOUT2, CLK2);
  brakeScale.set_scale();
  brakeScale.tare(); // Set scale to 0
  long zero_factor2 = brakeScale.read_average();

  // Set Range Values for Joystick
  Joystick.setThrottleRange(0, 1023);
  Joystick.setBrakeRange(0, 1023);

  // Start joystick connection
  Joystick.begin(testAutoSendMode);

}

void loop() {
  /* ------------ Read and set Values  ------------ */
  // Read load cell values
  rawThrottle = throttleScale.get_units();
  rawBrake = brakeScale.get_units();

  // Convert values
  adjThrottle = (mThrottle * rawThrottle) + cThrottle;
  adjBrake = (mBrake * rawBrake) + cBrake;

  // Write values to joystick
  Joystick.setThrottle(adjThrottle);
  Joystick.setBrake(adjBrake);


  /* ------------------ Debugging ----------------- */
  // Print to plotter
  #ifdef PLOT
    Serial.print(adjThrottle, 1);
    Serial.print(", ");
    Serial.println(adjBrake, 1);
  #endif

  // Timer counter
  #ifdef TIMER_COUNTER
    long currMillis = millis();
    loops++;

    if(currMillis - lastMillis > 1000){
      Serial.print("Loops last second:");
      Serial.println(loops);
     
      lastMillis = currMillis;
      loops = 0;
    } 
  #endif

}
