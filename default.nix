/*
{
  lib,
  stdenv,
  fetchFromGitHub,
  cmake,
  kconfig,
  kcoreaddons,
  kwidgetsaddons,
  wrapQtAppsHook,
  kdbusaddons,
  kde-cli-tools,
  plasma-workspace,
  qtstyleplugin-kvantum,
  qt6,
  imagemagick,
}:
*/

{
  pkgs ? (import <nixpkgs> { }),
}:

with pkgs;

stdenv.mkDerivation (finalAttrs: {
  pname = "qt-bug-QIcon-fromTheme-invisible-tray-icon";
  version = "0.0.1";
  src = ./.;
  /*
  postInstall = ''
    set -x
    cd .. # undo "cd build"
    pwd
    ls
    find . -name '*.png' -or -name '*.svg'
    mkdir -p $out/share/icons/hicolor/scalable/apps
    # install png icons
    cp src/resources/icons/koi*.png $out/share/icons/hicolor/scalable/apps
    pushd $out/share/icons/hicolor/scalable/apps
    # install svgz icons
    # ... but koi_tray.svgz is not used?
    find . -name '*.svg' | while read svg; do
      svgz="$svg"z
      [ -e "$svgz" ] && continue
      cat "$svg" | gzip >"$svgz"
    done
    # add white background to transparent png
    magick koi_tray.png -background white -alpha remove -alpha off koi_tray.png.2.png
    mv koi_tray.png.2.png koi_tray.png
    popd
  '';
  */
  nativeBuildInputs = [
    cmake
    qt6.wrapQtAppsHook
    imagemagick
  ];
  buildInputs = [
    /*
    kconfig
    kcoreaddons
    kwidgetsaddons
    qt6.qtsvg
    */
    qt6.qtbase
    qt6.qtsvg
  ];
})
