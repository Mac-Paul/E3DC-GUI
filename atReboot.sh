#!/bin/bash

sleep 12
sudo ./RscpGui/Key3 &
./RscpGui/RscpMain &
sleep 10
./RscpGui/RscpGui &
exit 0
