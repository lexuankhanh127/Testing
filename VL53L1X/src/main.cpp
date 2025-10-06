#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "SparkFun_VL53L1X.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display haha
SFEVL53L1X distanceSensor;
int bestDistance = 0;

void setup()
{
  // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  // initialize sensor
  if (distanceSensor.begin() != 0) // Begin returns 0 on a good init
  {
    lcd.print("Sensor failed");
    while (1)
      ;
  }
  distanceSensor.setDistanceModeLong();
  distanceSensor.setTimingBudgetInMs(200);
  distanceSensor.setIntermeasurementPeriod(250);
}

void loop()
{
  // Read distance
  distanceSensor.startRanging(); // Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  int distance = distanceSensor.getDistance(); // Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();
  if (distance > bestDistance)
  {
    bestDistance = distance;
  }

  // Print lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" mm");
  lcd.setCursor(0, 1);
  lcd.print("Best: ");
  lcd.print(bestDistance);
  lcd.print(" mm");
}