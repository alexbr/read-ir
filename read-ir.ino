/********************************************************

  Alex's IR reader
  Pin 7 to IR receiver out, IR V+ to 5v
  Pin 8 to 330 Ohm -> LED

*********************************************************/
#include <Wire.h>
#include <IRremote.h>

#define RECV_PIN 3

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // needed for native USB
  }

  // IR
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  checkIR();
}

void checkIR() {
  if (irrecv.decode(&results)) {
    dumpIR(&results);
    irrecv.resume(); // Receive the next value
  }
}

// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
// void * to work around compiler issue
//void dumpIR(void *v) {
//  decode_results *results = (decode_results *)v
void dumpIR(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.println("Could not decode message");
  }
  else {
    if (results->decode_type == NEC) {
      Serial.print("Decoded NEC: ");
    }
    else if (results->decode_type == SONY) {
      Serial.print("Decoded SONY: ");
    }
    else if (results->decode_type == RC5) {
      Serial.print("Decoded RC5: ");
    }
    else if (results->decode_type == RC6) {
      Serial.print("Decoded RC6: ");
    }
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
  }
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}
