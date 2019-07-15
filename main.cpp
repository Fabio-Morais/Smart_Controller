#include <SPI.h>
#include "SSD1306Spi.h"

SSD1306Spi display(D0, D2, D8);  // RES, DC, CS
 
void setup()   
{                
  Serial.begin(9600);
 init();
// Turn the display on
 displayOn(void);
// Write the buffer to the display memory
 display(void);
 drawString(2, 2, "ola");

 
}
 
 
void loop() 
{
 
}