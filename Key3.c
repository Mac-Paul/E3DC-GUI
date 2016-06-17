/*
gcc -g -o Key3  Key3.c -l wiringPi
*/

#include "wiringPi.h"

#define Key2 	5	// GPIO 24

int  main()
{
  sleep (10);
  if (wiringPiSetup () == -1)
    return (0) ;
    pinMode(Key3, INPUT);   // GPIO 24
    system("sudo gpio mode 5 up");

    while(1) {
      if (digitalRead(Key3)==0){
        delay(2000);
        if (digitalRead(Key2)==0){
          system("sudo shutdown -h now");
        }
      }
    }
}
