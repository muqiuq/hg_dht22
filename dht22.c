/*
 *      dht22.c:
 *	Simple test program to test the wiringPi functions
 *	Based on the existing dht11.c
 *	Amended by technion@lolware.net
 */

<<<<<<< HEAD
#include <wiringX.h>
=======
#include <wiringPi.h>
>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#include "locking.h"

#define MAXTIMINGS 85
<<<<<<< HEAD
static int DHTPIN = 1;
static int dht22_dat[5] = {0,0,0,0,0};

static void nullprint(int prio, const char *format_str, ...) {

}

=======
static int DHTPIN = 7;
static int dht22_dat[5] = {0,0,0,0,0};

>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59
static uint8_t sizecvt(const int read)
{
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0)
  {
<<<<<<< HEAD
    //printf("Invalid data from wiringPi library\n");
=======
    printf("Invalid data from wiringPi library\n");
>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59
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
<<<<<<< HEAD
  delayMicroseconds(10000);
  digitalWrite(DHTPIN, LOW);
  delayMicroseconds(18000);
=======
  delay(10);
  digitalWrite(DHTPIN, LOW);
  delay(18);
>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59
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


<<<<<<< HEAD
    //printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t );
    printf("{\"humidity\":\"%.2f\",\"temperature\":\"%.2f\"}\n", h, t );
=======
    printf("Humidity = %.2f %% Temperature = %.2f *C \n", h, t );
>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59
    return 1;
  }
  else
  {
<<<<<<< HEAD
    //printf("Data not good, skip\n");
=======
    printf("Data not good, skip\n");
>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59
    return 0;
  }
}

int main (int argc, char *argv[])
{
<<<<<<< HEAD
  wiringXLog = nullprint;
  int lockfd;
  int tries = 30;

  if (argc < 2) {
    /*printf ("usage: %s <pin> (<tries>)\ndescription: pin is the wiringPi pin number\nusing 7 (GPIO 4)\nOptional: tries is the number of times to try to obtain a read (default 100)",argv[0]);*/
  }
=======
  int lockfd;
  int tries = 100;

  if (argc < 2)
    printf ("usage: %s <pin> (<tries>)\ndescription: pin is the wiringPi pin number\nusing 7 (GPIO 4)\nOptional: tries is the number of times to try to obtain a read (default 100)",argv[0]);
>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59
  else
    DHTPIN = atoi(argv[1]);
   

  if (argc == 3)
    tries = atoi(argv[2]);

  if (tries < 1) {
<<<<<<< HEAD
    exit(EXIT_FAILURE);
  }

  //printf ("Raspberry Pi wiringPi DHT22 reader\nwww.lolware.net\n") ;

  lockfd = open_lockfile(LOCKFILE);

  if (wiringXSetup () == -1)
=======
    printf("Invalid tries supplied\n");
    exit(EXIT_FAILURE);
  }

  printf ("Raspberry Pi wiringPi DHT22 reader\nwww.lolware.net\n") ;

  lockfd = open_lockfile(LOCKFILE);

  if (wiringPiSetup () == -1)
>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59
    exit(EXIT_FAILURE) ;
	
  if (setuid(getuid()) < 0)
  {
    perror("Dropping privileges failed\n");
    exit(EXIT_FAILURE);
  }

  while (read_dht22_dat() == 0 && tries--) 
  {
<<<<<<< HEAD
     delayMicroseconds(1000000); // wait 1sec to refresh
  }

  delayMicroseconds(1500000);
=======
     delay(1000); // wait 1sec to refresh
  }

  delay(1500);
>>>>>>> e3656bfd5c9676408f741b62e4bcb78bb4722a59
  close_lockfile(lockfd);

  return 0 ;
}
