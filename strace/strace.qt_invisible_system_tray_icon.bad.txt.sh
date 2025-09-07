#!/bin/sh

timeout 5 strace -s999 ./result/bin/qt_invisible_system_tray_icon &> strace.qt_invisible_system_tray_icon.bad.txt
