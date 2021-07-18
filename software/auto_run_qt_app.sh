#!/bin/bash
echo raedy run qt app
mount /dev/mmcblk0p1 /mnt
cp -rf /mnt/test_qt/. /opt/my_qt_apps
echo copy file finished...
cd /opt/my_qt_apps/
./Bldc_Fan
