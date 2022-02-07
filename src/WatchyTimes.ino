#include "WatchyTimes.h"

WatchyTimes watchy;

void setup()
{
  Serial.begin(115200);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port on Arduino only
  }
  Serial.println("*****");
  
  watchy.init();
}

void loop(){}

