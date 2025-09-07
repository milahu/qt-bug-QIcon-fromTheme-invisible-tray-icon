#!/bin/sh

timeout 5 strace -s999 ./result/bin/qt_invisible_system_tray_icon asdf &> strace.qt_invisible_system_tray_icon.good.txt
