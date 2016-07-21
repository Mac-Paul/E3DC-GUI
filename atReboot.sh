#!/bin/bash

sleep 12
sudo /home/pi/RscpGui/Key3 &
/home/pi/RscpGui/RscpMain &
sleep 10
/home/pi/RscpGui/RscpGui &
/home/pi/RscpGui/watchdog &
exit 0
