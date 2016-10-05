
#ifndef _RTC_H_
#define _RTC_H_

#include <TimeLib.h>

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

/*  code to process time sync messages from the serial port   */
char TIME_HEADER[] = "T";   // Header tag for serial time sync message

unsigned long processSyncMessage() 
{
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) 
  {
     pctime = Serial.parseInt();
    
     if( pctime < DEFAULT_TIME)  // check the value is a valid time (greater than Jan 1 2013)
     {
       pctime = 0L; // return 0 to indicate that the time is not valid
     }
  }
  
  return pctime;
}

void syncRTC()
{
  if (Serial.available()) 
  {
	time_t t = processSyncMessage();
    if (t != 0) 
	{
      Teensy3Clock.set(t); // set the RTC
      setTime(t);
    }
  }
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  if(digits < 10) Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay() 
{
  // digital clock display of the time
  printDigits(hour());
  Serial.print(":");
  printDigits(minute());
  Serial.print(":");
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(year()); 
  Serial.println(); 
}

#endif
