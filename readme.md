# qt-invisible-system-tray-icon

is this a Qt bug...?

- **QIcon::fromTheme** + QSystemTrayIcon &rarr; **invisible** system tray icon
- **QIcon** + QSystemTrayIcon &rarr; **visible** system tray icon

bad:

```cc
QIcon fallbackIcon = QIcon(":/resources/icons/koi_tray.png");
icon = QIcon::fromTheme("koi_tray", fallbackIcon);
```

good:

```cc
icon = QIcon(":/resources/icons/koi_tray.png");
```

this minimal repro is based on
[Koi/src/trayManager.cpp](https://github.com/baduhai/Koi/blob/11a50175ce7bf37145e5c13d579e97a85a9bdade/src/trayManager.cpp#L11)

```cc
QIcon icon = QIcon::fromTheme(
    "koi_tray",
    QIcon(":/resources/icons/koi_tray.png")
);
```

## build

```
nix-build
```

or

```
cmake . && make
```

## run

[src/qt_invisible_system_tray_icon.cpp](src/qt_invisible_system_tray_icon.cpp)

this breaks: it shows an invisible tray icon

```
qt_invisible_system_tray_icon
```

this works: it shows the [koi_tray.png](src/resources/icons/koi_tray.png) tray icon

```
qt_invisible_system_tray_icon asdf
```

## debug

[src/qt_find_icons.cpp](src/qt_find_icons.cpp)

```
$ qt_find_icons
Searching for icon name: "koi_tray"
QIcon theme name: "breeze"
QIcon theme search paths:
  /home/user/.local/share/icons
  /nix/store/d9xgvsah5g04sq33hvr52ahwddp84l8v-qt-bug-QIcon-fromTheme-invisible-tray-icon-0.0.1/share/icons
  /nix/store/8ahhl2p7chd2zzl2p7x3g91fvspzcida-breeze-icons-6.15.0/share/icons
  /nix/store/7vgf0b2acfb82c8c7zg5hkb6b39wjnjl-cups-2.4.11/share/icons
  /nix/store/2gqw3wp905k3w4c1s3j4mwdy00yxys3p-plasma-workspace-6.4.0/share/icons
  /nix/store/5fxsv081nn846i333p69wjnr6ndpncs5-kwin-6.4.0/share/icons
  /nix/store/96wqq7dxi9gl7b3amjfk3wj9rwfkyrs0-breeze-6.4.0/share/icons
  /nix/store/j4d0gsk5qbixm5hnpwffbr0bzdzgl2s4-oxygen-icons-6.1.0/share/icons
  /run/current-system/sw/share/icons
  /home/user/.icons
  :/icons
Found candidates:
 - "/nix/store/d9xgvsah5g04sq33hvr52ahwddp84l8v-qt-bug-QIcon-fromTheme-invisible-tray-icon-0.0.1/share/icons/hicolor/scalable/apps/koi_tray.svg"
 - "/nix/store/d9xgvsah5g04sq33hvr52ahwddp84l8v-qt-bug-QIcon-fromTheme-invisible-tray-icon-0.0.1/share/icons/hicolor/scalable/apps/koi_tray.png"
 - ":/resources/icons/koi_tray.svg"
 - ":/resources/icons/koi_tray.png"
```
