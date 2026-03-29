#include <IRremote.h>

//Pin uses
const int RECV_PIN = 6; // this is the signal pin of the IR receiver
const int ledPin = 8;

unsigned long ledOnTime = 0;   // Stores when the LED was turned on every the right button is pressed
bool ledIsActive = false;      // Tracks if the LED is currently glowing
const long duration = 4000;    // Auto-off after 4 seconds (if no stop signal)

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

  Serial.println(command) output the the button command of the pressed button
  use it to tweak the " if statement"

  Also millis() function use instead of delay for system responsiveness.

  */

  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    Serial.print("Command: ");
    Serial.println(command);
    // Serial.println(command, HEX);

    // COMMAND TO TURN ON (e.g., 0x2).
    if (command == 0x2) {
      digitalWrite(ledPin, HIGH);
      ledIsActive = true;
      ledOnTime = millis(); // Record the start time
      Serial.println("LED ON");
    }

    // COMMAND TO TURN OFF (e.g., 0x5 - Change this to your preferred button)
    if (command == 0x1) {
      digitalWrite(ledPin, LOW);
      ledIsActive = false;
      Serial.println("LED OFF (Manual)");
    }

    IrReceiver.resume();
  }

  // AUTO-OFF LOGIC: Check if time has expired without using delay()
  if (ledIsActive && (millis() - ledOnTime >= duration)) {
    digitalWrite(ledPin, LOW);
    ledIsActive = false;
    Serial.println("LED OFF (Timeout)");
  }
}
