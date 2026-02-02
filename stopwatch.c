#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  lcd.init();         // initialize the lcd 
  lcd.backlight();    // Turn on the backlight
  lcd.setCursor(0,0);
  lcd.print("Hello, World!");
}

void loop() {
  // Get the total elapsed time in seconds
  unsigned long totalSeconds = millis() / 1000;

  // Calculate hours, minutes, and seconds
  int h = (totalSeconds / 3600) % 24;
  int m = (totalSeconds / 60) % 60;
  int s = totalSeconds % 60;

  // Set cursor to the second row
  lcd.setCursor(0, 1);

  // Print Hours
  if (h < 10) lcd.print('0'); // Leading zero
  lcd.print(h);
  lcd.print(":");

  // Print Minutes
  if (m < 10) lcd.print('0'); // Leading zero
  lcd.print(m);
  lcd.print(":");

  // Print Seconds
  if (s < 10) lcd.print('0'); // Leading zero
  lcd.print(s);

  delay(200); // Small delay to prevent the LCD from flickering
}
