#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ==========================================
// 1. OLED Configuration
// ==========================================
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ==========================================
// 2. MPU6050 Configuration
// ==========================================
const int MPU_ADDR = 0x68; 
int16_t accX, accY, accZ;
int16_t gyroX, gyroY, gyroZ;
float pitch = 0;
float roll = 0;
unsigned long previousTime = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // --- Initialize OLED ---
  // If your screen stays blank, ensure you are using 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  
  // --- Initialize MPU6050 ---
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // Power Management 1 register
  Wire.write(0);    // Write 0 to wake it up
  Wire.endTransmission(true);
  
  previousTime = millis();
}

void loop() {
  // --------------------------------------------------
  // 1. Read Accelerometer Data
  // --------------------------------------------------
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); 
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU_ADDR, 6, true); 
  
  accX = (Wire.read() << 8 | Wire.read()); 
  accY = (Wire.read() << 8 | Wire.read()); 
  accZ = (Wire.read() << 8 | Wire.read()); 
  
  // --------------------------------------------------
  // 2. Read Gyroscope Data
  // --------------------------------------------------
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); 
  
  gyroX = (Wire.read() << 8 | Wire.read());
  gyroY = (Wire.read() << 8 | Wire.read());
  gyroZ = (Wire.read() << 8 | Wire.read()); 

  // --------------------------------------------------
  // 3. Calculate Time Elapsed (dt)
  // --------------------------------------------------
  unsigned long currentTime = millis();
  float dt = (currentTime - previousTime) / 1000.0; 
  previousTime = currentTime;

  // --------------------------------------------------
  // 4. Convert Raw Values & Filter
  // --------------------------------------------------
  float accAngleX = atan2(accY, accZ) * 180.0 / PI;
  float accAngleY = atan2(-accX, sqrt(pow(accY, 2) + pow(accZ, 2))) * 180.0 / PI;

  float gyroRateX = gyroX / 131.0;
  float gyroRateY = gyroY / 131.0;

  pitch = 0.98 * (pitch + gyroRateX * dt) + 0.02 * accAngleX;
  roll  = 0.98 * (roll  + gyroRateY * dt) + 0.02 * accAngleY;

  // --------------------------------------------------
  // 5. Update the OLED Display (Two-Color Screen Fix)
  // --------------------------------------------------
  display.clearDisplay(); 
  
  // --- 1. THE YELLOW ZONE (Top 16 Pixels) ---
  // We use this strictly for our labels so they don't cross the gap
  display.setTextSize(1);
  display.setCursor(15, 4); // Positioned over the left column
  display.print(F("PITCH"));
  
  display.setCursor(85, 4); // Positioned over the right column
  display.print(F("ROLL"));

  // Draw a horizontal line exactly at the physical gap to hide it!
  display.drawLine(0, 16, 128, 16, SSD1306_WHITE);
  
  // Draw a vertical line down the middle of the blue section
  display.drawLine(64, 16, 64, 64, SSD1306_WHITE);

  // --- 2. THE BLUE ZONE (Bottom 48 Pixels) ---
  // We drop to Size 2 so both numbers can fit side-by-side
  display.setTextSize(2);
  
  // Pitch Value (Left Column)
  display.setCursor(5, 35); // Pushed down to visually center it in the blue area
  display.print(pitch, 0);
  display.print((char)247); // Degree symbol
  
  // Roll Value (Right Column)
  display.setCursor(72, 35); 
  // display.print(roll-5, 0);
  display.print(-(roll-5), 0);
  display.print((char)247); // Degree symbol

  // Blast it to the screen
  display.display();
}
