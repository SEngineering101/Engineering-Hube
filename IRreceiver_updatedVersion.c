#include <IRremote.h>

//Pin uses
const int RECV_PIN = 6; // this is the signal pin of the IR receiver
const int ledPin = 8;

unsigned long ledOnTime = 0;   // Stores when the LED was turned on every the right button is pressed
bool ledIsActive = false;      // Tracks if the LED is currently glowing
const long duration = 5000;    // Auto-off after 5 seconds (if no stop signal)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("Ready to receive...");
}

void loop() {

  /*
  serial print are added for debugging, They can also be use to map
  your remote first (determine the role of each button of the remote)
  Also millis() function use instead of delay for system responsiveness.

  */

  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    
    // COMMAND TO TURN ON (e.g., 0x2)
    if (command == 0x2) {
      if (!ledIsActive){
        digitalWrite(ledPin, HIGH);
        ledIsActive = true;
        ledOnTime = millis(); // Record the start time
        Serial.println("LED ON");
      }else{
        digitalWrite(ledPin, LOW);
        ledIsActive = false;
        Serial.println("LED OFF (Manual)");
      }
    }
    delay(60);

    IrReceiver.resume();
  }

  // AUTO-OFF LOGIC: Check if time has expired without using delay()
  if (ledIsActive && (millis() - ledOnTime >= duration)) {
    digitalWrite(ledPin, LOW);
    ledIsActive = false;
    Serial.println("LED OFF (Timeout)");
  }
}
