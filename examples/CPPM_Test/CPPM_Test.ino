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
  
  StartCPPM(0);  // interupt 0 is pin 3 on Micro

}

void loop() {
  
  delay(250);
  
  CPPM_display();
  
}
