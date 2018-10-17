/*
 * CPPM Test
 *
 * Connect receiver output to pin 3 on arduino micro
 * Displayes pulse width of each channel
 */

#include <CPPM-RX.h>

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("*** Program Start ***");

  StartCPPM(3);  // pin 3 is interupt 0 on Micro
  // ( refernce for pins with intterupts: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

}

void loop() {

  delay(250);

  CPPM_display();

}
