#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>

//Zeitdifferenz zur aktuellen Zeit bis zur Watchdog aktivierung, in Sekunden
#define diff            300
//Zeitinterval für die Abfragen des Watchdog, in Sekunden
#define sleepTime       120
//Anzahl Programm Neustarts bis zum Reboot
#define rebootCounter   4
//Nach dieser Zeit wird der rebootCounter zurückgesetzt wenn die Daten aktuell sind, in Minuten
#define resetMin        60

using namespace std;

char response[20], delete_res[20], Path[100], DATE[40], TIME[40];
int UnixTime, resetTime;
int counterReboot = 0;
int resetCounter = 0;

void ReadData(char filePath[100]){
  fstream datei(filePath);
  if (datei.is_open()) {
    datei.getline(delete_res ,20, '\n');
    datei.getline(response ,20, '\n');
    datei.close();
  }
  else cerr << "Konnte Datei nicht erstellen!";
}


int main()
{
    resetTime = resetMin *60 / sleepTime;

    while(1){
      sleep(sleepTime);
      int AktuallTime = time(NULL);
      time_t timestamp;
      tm *sys;
      timestamp = time(NULL);
      sys = localtime(&timestamp);
      strftime (DATE,40,"%d.%m.%Y",sys);
      strftime (TIME,40,"%H:%M:%S",sys);
      snprintf (Path, (size_t)100, "/mnt/RAMDisk/Unixtime.txt");
      ReadData(Path);
      int UnixTime = atoi(response);
      int DiffTime = AktuallTime - UnixTime;
      if(DiffTime > diff){
        counterReboot ++;
        if (counterReboot == rebootCounter){
          ofstream fout("/home/pi/RscpGui/Watchdog.csv", ios::app);
          if (fout.is_open()) {
            fout << DATE << ";" << TIME << ";" << AktuallTime << ";" << UnixTime << ";" << resetCounter << ";" << "reboot" << endl;
            fout.close();
          }
          system("sudo reboot");
          return(0);
        }
        system("pkill RscpMain");
        sleep(2);
        cout << "test";
        system("/home/pi/RscpGui/RscpMain &");
        ofstream fout("/home/pi/RscpGui/Watchdog.csv", ios::app);
        if (fout.is_open()) {
          fout << DATE << ";" << TIME << ";" << AktuallTime << ";" << UnixTime << ";" << resetCounter << ";" << "pkill" << endl;
          fout.close();
        }
      }
      else {
        resetCounter ++;
        if(resetCounter == resetTime){
          counterReboot = 0;
          resetCounter = 0;
        }
      }
      cout << DATE << " ; " << TIME << " ; " << AktuallTime << " ; " << UnixTime << " ; " << resetCounter << " ; " << counterReboot << " \n" ;
    }
}
