#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <VL53L1X.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
VL53L1X sensor;
uint16_t bestDistance = 0;

void setup()
{
  // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("VL53L1X Ready");
  // initialize sensor
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000);
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Fail!");
    while (1)
      ;
  }
  sensor.setDistanceMode(VL53L1X::Short);
  sensor.setMeasurementTimingBudget(50000);
  sensor.startContinuous(50);
}

void loop()
{
  uint16_t distance = sensor.read();
  if (!sensor.timeoutOccurred() && distance > bestDistance)
  {
    bestDistance = distance;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  Serial.print("Distance: ");
  Serial.println(distance);
  lcd.print(" mm");
  lcd.setCursor(0, 1);
  lcd.print("Best: ");
  lcd.print(bestDistance);
  lcd.print(" mm");
  if (sensor.timeoutOccurred())
  {
    lcd.setCursor(0, 1);
    lcd.print("TIMEOUT");
  }
  delay(200);
}