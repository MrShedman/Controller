

  #if ARDUINO < 100
	#include <WProgram.h>
  #else
	#include <Arduino.h>
  #endif

  #include <stddef.h>
  
  #include <stdint.h>
  #include <stdio.h>
  #include <string.h>

  #include <SPI.h>

//#define SPI_USE_BUS1

#ifdef SPI_USE_BUS1
  #define _SPI SPI1
#else
  #define _SPI SPI
#endif

  #define printf Serial.printf
  
  #ifdef SERIAL_DEBUG
	#define IF_SERIAL_DEBUG(x) ({x;})
  #else
	#define IF_SERIAL_DEBUG(x)
  #endif
  

  #define PRIPSTR "%s"




