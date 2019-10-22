#include "IRremote.h"

IRrecv irrecv(4); // Receive on pin 4
decode_results results;

void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  Serial.println( F("IR Receiver Ready...") );
  Serial.println();

  delay(1000);
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Continue receiving
  }
}
