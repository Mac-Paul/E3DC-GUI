## RSCP GUI
Hier beschreibe ich wie du auf einen Rasperry Pi mit 3,2" Touchdisplay dein S10 Hauskraftwerk von E3DC visualisieren kannst.

Als Schnittstelle zwischen S10 und dem Raspberry dient ein Beispielapplikation, die E3DC im Downloadbereich hat.

Ich nutze diese Applikation auf einem Raspberry Pi 3. Mein S10-E hat Release-Version 2016-02. Diese Applikation funktioniert auch für S10-Mini und für S10-SP40. Mit meiner sehr geringen Erfahrung in der Programmierung sind die Änderungen in der Applikation eventuell unprofessionell ausgefallen, aber die Funktionalität wird erfüllt.

<img src="https://s20.postimg.org/90o1nmhy5/E3_DC_GUI.jpg" alt="E3DC-GUI">

### Info

Die Funktion der E3DC-GUI besteht aus zwei Programmen.

1. __RscpMain__  Die RSCP-Beispielapplication geschrieben in C++ von E3/DC
2. __RscpGui__   Ein einfaches Display Programm in C

Das RSCP Programm holt per E3DC-Protokoll die Daten vom S10 und schreibt die Werte in eine Text-Datei in das RAMDisk des Raspberry Pi.

Die GUI ließt die Text-Datei aus dem RAMDisk und baut die GUI.

Beide Programme liegen nach dem Folgenden Download in diesem Ordner:
```
/pi/home/RscpGui/
```

### Download

Mit dem Git-Clone-Befehl wird der SourceCode auf den Raspberry kopiert.
```shell
git clone git://github.com/nischram/E3DC-GUI.git ~/RscpGui
```

### ToDo RscpMain.cpp

In der _RscpMain.c_ musst du die Zugangsdaten zum S10 noch definieren. Die Einstellungen sind in Zeile 22-25 vorzunehmen:
```
#define SERVER_IP           "192.168.xxx.xxx"
#define E3DC_USER           "Max.Mustermann"
#define E3DC_PASS           "password"
#define AES_PASS            "1234567890"
```
Die IP-Adresse ist die IP von deinem S10, E3DC_USER ist der Benutzername vom Kundenportal, E3DC_PASS ist dein Password zum Kundenportal und AES_PASS ist ein RSCP-Passwort welches du am S10 vergeben kannst. Im „Hauptmenü“ unter „Einstellungen“ kann das RSCP-Passwort gesetzt werden.
Um die Einstellungen vorzunehmen kann man mit "WSCP" oder ähnlichen Programmen die Datei öffenen.
Das wechsel in den Ordner nicht vergessen:
```
cd RscpGui
```
Nach dem Einstellen muss das Programm mit:
```
make
```
compiliert werden.

Ein Test der RscpMain führt man mit diesem Befehl aus:
```
./RscpMain
```
Die Ausgabe sollte wie folgt aussehen:
```
pi@raspberrypi:~/RscpGui $ ./RscpMain
Connecting to server 192.168.xxx.xxx:5033
Connected successfully

Request authentication
RSCP authentitication level 10

Request cyclic example data
EMS PV power is 34 W
EMS BAT power is -834 W
EMS house power is 873 W
EMS grid power is 5 W
Battery SOC is 72.8 %
```
Gestoppt wird die Applikation vorerst mit „strg“ + „c“

### ToDo RscpGui.c

Die GUI-Datei braucht nur noch kompiliert werden, Änderungen sind nicht nötig.
```
gcc -g -o RscpGui  RscpGui.c
```

### Autostart der Applikation (durch Crontab-Job)
Damit das Programm und die Variablen nun dauerhaft genutzt werden können, muss die Applikation auf dem Raspberry in den Autostart gebracht werden. Diese geschieht bei einem Raspberry Pi in dem ein Crontab-Job erstellt wird.

Die Crontab ruft man auf mit:
```shell
pi@raspberrypi ~/RscpGui $  crontab -e
```
- Crontab für die Bearbeitung öffnen

In der geöffneten Crontab wird eine neue Zeile mit diesem Inhalt eingefügt:
```shell
@reboot /home/pi/RscpGui/atReboot.sh
```
Die Änderung wird mit „ctrl“ + „o“ gespeichert und die Crontab beendet mit
„ctrl“ + „x“.
Weitere Informationen zur Crontab entnehmen Sie bitte aus Quellen wie z. B. dieser:
[https://raspberry.tips/raspberrypi-einsteiger/cronjob-auf-dem-raspberry-pi-einrichten](https://raspberry.tips/raspberrypi-einsteiger/cronjob-auf-dem-raspberry-pi-einrichten)

In der atReboot.sh wird mit einer Verzögerung die RscpMain und danach die RscpGui gestartet.
Die atReboot.sh muss noch ausführbar gemacht werden, mit:
```
chmod +x atReboot.sh
```

### Beenden

Damit du den Raspberry auch mit gestarteter Applikation ausschalten kannst, habe ich ein Programm erstellt, welches die 3.Taste meines Displays abfrgt. Nach einem druck von 2 Sekunden, fährt der Raspberry runter.
Die Datei hierfür heißt Key3.c auch diese muss Compiliert werden:
```
gcc -g -o Key3  Key3.c -l wiringPi
```

### RAMDisk

RAMDisk am Raspberry erstellen:
Zuerst wird ein Mountpoint für die RAM-Disk erzeugt:
```
sudo mkdir /mnt/RAMDisk
```
Für die RAM-Disk muß die Filesystem Table angepasst werden:
```
sudo nano /etc/fstab
```
Einfügen dieser Zeile am Ende der Datei:
```
tmpfs /mnt/RAMDisk tmpfs nodev,nosuid,size=4M 0 0
```
Die Größe wird über den Parameter "4M" auf 4 MB festgelegt. Jetzt montiert man alle Filesysteme über:
```
sudo mount -a
```
Der Erfolg lässt sich mit Diskfree überprüfen:
```
df
```
Es sollte dann ein Eintrag mit der RAM-Disk zu finden sein:
```
Filesystem 1K-blocks Used Available Use% Mounted on
rootfs 15071704 2734624 11674436 19% /
/dev/root 15071704 2734624 11674436 19% /
devtmpfs 218620 0 218620 0% /dev
tmpfs 44580 236 44344 1% /run
tmpfs 5120 0 5120 0% /run/lock
tmpfs 89140 0 89140 0% /run/shm
/dev/mmcblk0p1 57288 19712 37576 35% /boot
tmpfs 4096 0 4096 0% /mnt/RAMDisk
```

Diesen Teil zum RAMDisk habe ich von hier Kopiert:
[http://www.kriwanek.de/raspberry-pi/486-ram-disk-auf-raspberry-pi-einrichten.html](http://www.kriwanek.de/raspberry-pi/486-ram-disk-auf-raspberry-pi-einrichten.html)

### Deiteibeschreibung

AES.cpp / AES.h / RscpProtokoll.cpp / RscpTag.h / RscpType.h / SocketConnektion.cpp / SocketConnection.h
gehören zur RSCP-Applikation.

font_8x8.c / framebuffer.c / touch.c / touch.h
gehören zur GUI und ich habe den Ursprung dieser Datein hier her:
[http://ozzmaker.com/controlling-the-gpio-on-a-raspberry-pi-with-a-touchscreen/](http://ozzmaker.com/controlling-the-gpio-on-a-raspberry-pi-with-a-touchscreen/)

### Änderungen

Änderungen an meiner GUI dürfen gerne Vorgenommen und hier eingefügt werden.

### Nutzen der RSCP-Applikation

Die RSCP-Applikation kann für viele weitere Anwendungen genutzt werden. Wie zum Beispiel mein [E3DCtoHM](https://github.com/nischram/E3DCtoHM.git) um eine HomeMatic mit Werten des S10 zu versorgen.

Wenn ihr eigene Projekte erstellt, würde ich mich freun, wenn ihr diese veröffentlicht.
