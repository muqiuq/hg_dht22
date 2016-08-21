/*
 *      dht22.c:
 *	Simple test program to test the wiringX functions
 *	Based on the existing dht11.c
 *	Amended by technion@lolware.net
 *  Adapted for wiringX by philipp@uisa.ch
 */

#include <wiringX.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#include "locking.h"

#define MAXTIMINGS 85
static int DHTPIN = 1;
static int dht22_dat[5] = {0,0,0,0,0};

//#define DEBUG 

static void nullprint(int prio, const char *format_str, ...) {

}

static uint8_t sizecvt(const int read)
{
  /* digitalRead() and friends from wiringX are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0)
  {
    #ifdef DEBUG
      printf("Invalid data from wiringX library\n");
    #endif
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}

static int read_dht22_dat()
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  // pull pin down for 18 milliseconds
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(10000);
  digitalWrite(DHTPIN, LOW);
  delayMicroseconds(18000);
  // then pull it up for 40 microseconds
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(40); 
  // prepare to read the pin
  pinMode(DHTPIN, INPUT);

  // detect change and read data
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (sizecvt(digitalRead(DHTPIN)) == laststate) {
      counter++;
      delayMicroseconds(1);
      if (counter == 255) {
        break;
      }
    }
    laststate = sizecvt(digitalRead(DHTPIN));

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      dht22_dat[j/8] <<= 1;
      if (counter > 16)
        dht22_dat[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((j >= 40) && 
      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
        float t, h;
        h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
        h /= 10;
        t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
        t /= 10.0;
        if ((dht22_dat[2] & 0x80) != 0)  t *= -1;


    printf("{\"humidity\":\"%.2f\",\"temperature\":\"%.2f\"}\n", h, t );
    return 1;
  }
  else
  {
    #ifdef DEBUG
      printf("Data not good, skip\n");
    #endif
    return 0;
  }
}

int main (int argc, char *argv[])
{
  wiringXLog = nullprint;
  int lockfd;
  int tries = 30;

  if (argc < 2) {
    #ifdef DEBUG
      printf ("usage: %s <pin> (<tries>)\ndescription: pin is the wiringX pin number\nusing 4 (GPIO 72)\nOptional: tries is the number of times to try to obtain a read (default 100)",argv[0]);
    #endif
  }
  else
    DHTPIN = atoi(argv[1]);
   

  if (argc == 3)
    tries = atoi(argv[2]);
#ifdef DEBUG
  else
    printf("Set tries to default %i", tries);
#endif
  if (tries < 1) {
    exit(EXIT_FAILURE);
  }

  lockfd = open_lockfile(LOCKFILE);

  if (wiringXSetup () == -1)
    exit(EXIT_FAILURE) ;
	
  if (setuid(getuid()) < 0)
  {
    perror("Dropping privileges failed\n");
    exit(EXIT_FAILURE);
  }

  while (read_dht22_dat() == 0 && tries--) 
  {
     delayMicroseconds(1000000); // wait 1sec to refresh
  }

  delayMicroseconds(1500000);
  close_lockfile(lockfd);

  return 0 ;
}
